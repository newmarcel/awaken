//
//  IOPowerSource.cpp
//  awaken
//
//  Created by Marcel Dierkes on 07.05.20.
//  Copyright © 2020 Marcel Dierkes. All rights reserved.
//

#include "IOPowerSource/IOPowerSource.hpp"
#include <CoreFoundation/CoreFoundation.h>
#include <dispatch/dispatch.h>
#include <notify.h>
#include <IOKit/ps/IOPSKeys.h>
#include <IOKit/ps/IOPowerSources.h>
#include "Log.hpp"

using namespace std;
using namespace Awaken;

#pragma mark - Life Cycle

IOPowerSource::IOPowerSource() noexcept
    : _capacity(CapacityUnavailable)
    , _capacityChangeHandler(nullopt)
    , _dispatchQueue(nullptr)
    , _notificationToken(0)
{
}

IOPowerSource::~IOPowerSource() noexcept
{
    this->unregisterFromCapacityChanges();
}

#pragma mark - Power Source Infos

namespace Awaken
{

/// Returns a retained power source dictionary copy,
/// call `CFRelease()` when done!
static auto CopyPowerSourceDescription() -> optional<CFDictionaryRef>
{
    auto powerSources = IOPSCopyPowerSourcesInfo();
    auto sourcesList = IOPSCopyPowerSourcesList(powerSources);
    
    if(CFArrayGetCount(sourcesList) == 0)
    {
        CFRelease(sourcesList);
        CFRelease(powerSources);
        os_log(DefaultLog, "No power sources found.");
        return nullopt;
    }
    
    auto powerSource = IOPSGetPowerSourceDescription(sourcesList, CFArrayGetValueAtIndex(sourcesList, 0));
    CFRetain(powerSource);
    
    CFRelease(sourcesList);
    CFRelease(powerSources);
    
    if(powerSource == nullptr) { return nullopt; }
    return powerSource;
}

}

#pragma mark - Battery Capacity

bool IOPowerSource::hasBattery() const noexcept
{
    if(auto powerSourceDescription = CopyPowerSourceDescription())
    {
        const auto key = CFSTR(kIOPSTypeKey);
        const auto internalBatteryTypeKey = CFSTR(kIOPSInternalBatteryType);
        
        auto batteryType = static_cast<CFStringRef>(CFDictionaryGetValue(*powerSourceDescription, key));
        if(batteryType == nullptr)
        {
            CFRelease(*powerSourceDescription);
            return false;
        }
        
        const bool isInternalBattery = CFEqual(batteryType, internalBatteryTypeKey);
        CFRelease(*powerSourceDescription);
        
        os_log(DefaultLog, "Device has internal battery: %{public}d", isInternalBattery);
        return isInternalBattery;
    }
    else
    {
        return false;
    }
}

float IOPowerSource::capacity() const noexcept
{
    if(auto powerSourceDescription = CopyPowerSourceDescription())
    {
        const auto key = CFSTR(kIOPSCurrentCapacityKey);
        const auto capacity = static_cast<CFNumberRef>(CFDictionaryGetValue(*powerSourceDescription, key));
        if(capacity == nullptr)
        {
            CFRelease(*powerSourceDescription);
            return CapacityUnavailable;
        }
        
        float capacityValue = CapacityUnavailable;
        CFNumberGetValue(capacity, kCFNumberFloatType, &capacityValue);
        CFRelease(*powerSourceDescription);
        
        os_log(DefaultLog, "Battery Capacity: %{public}.00f", capacityValue);
        return capacityValue;
    }
    else
    {
        return CapacityUnavailable;
    }
}

#pragma mark - Capacity Changes

void IOPowerSource::setCapacityChangeHandler(std::function<void(float)>&& capacityChangeHandler) noexcept
{
    this->_capacityChangeHandler = capacityChangeHandler;
}

bool IOPowerSource::registerForCapacityChanges() noexcept
{
    if(this->_notificationToken != 0)
    {
        os_log(DefaultLog, "Already registered for capacity changes.");
        return false;
    }
    
    const auto dispatchQueue = dispatch_queue_create("info.marcel-dierkes.Awaken.IOPowerSourceQueue",
                                                     DISPATCH_QUEUE_SERIAL);
    this->_dispatchQueue = dispatchQueue;
    
    auto capacityDidChange = [this](){
        const auto capacity = this->capacity();
        
        if(capacity != this->_capacity)
        {
            os_log(DefaultLog, "Capacity did change… %{public}.00f", capacity);
            this->_capacity = capacity;
            
            if(const auto& capacityChangeHandler = this->_capacityChangeHandler)
            {
                (*capacityChangeHandler)(capacity);
            }
        }
        else
        {
            os_log(DefaultLog, "Capacity did NOT change… %{public}.00f", capacity);
        }
    };
    
    notify_register_dispatch(kIOPSTimeRemainingNotificationKey, &this->_notificationToken, dispatchQueue, ^(int token) {
        capacityDidChange();
    });
    
    return true;
}

bool IOPowerSource::unregisterFromCapacityChanges() noexcept
{
    if(this->_notificationToken == 0)
    {
        os_log(DefaultLog, "Not registered for capacity changes.");
        return false;
    }
    else
    {
        notify_cancel(this->_notificationToken);
        this->_notificationToken = 0;
    }
    
    this->_capacity = CapacityUnavailable;
    
    const auto dispatchQueue = static_cast<dispatch_queue_t>(this->_dispatchQueue);
    dispatch_release(dispatchQueue);
    this->_dispatchQueue = nullptr;
    
    return true;
}
