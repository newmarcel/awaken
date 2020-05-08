//
//  IOPowerSource.cpp
//  awaken
//
//  Created by Marcel Dierkes on 07.05.20.
//  Copyright © 2020 Marcel Dierkes. All rights reserved.
//

#include "IOPowerSource/IOPowerSource.hpp"
#include <CoreFoundation/CoreFoundation.h>
#import <IOKit/ps/IOPSKeys.h>
#import <IOKit/ps/IOPowerSources.h>
#include "Log.hpp"

using namespace std;
using namespace Awaken;

#pragma mark - Life Cycle

IOPowerSource::IOPowerSource() noexcept
    : _capacityChangeHandler(nullopt)
    , _runLoopSource(nullptr)
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
    
    if(powerSource == NULL) { return nullopt; }
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
        if(batteryType == NULL)
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
        
        os_log(DefaultLog, "Battery Capacity: %{public}f", capacityValue);
        return capacityValue;
    }
    else
    {
        return CapacityUnavailable;
    }
}

#pragma mark - Capacity Changes

void IOPowerSource::_batteryCapacityDidChange(void* context)
{
    os_log(DefaultLog, "Capacity did change…");
    
    const auto powerSource = static_cast<IOPowerSource *>(context);
    
    if(const auto& capacityChangeHandler = powerSource->_capacityChangeHandler)
    {
        const auto capacity = powerSource->capacity();
        (*capacityChangeHandler)(capacity);
    }
}

void IOPowerSource::setCapacityChangeHandler(std::function<void(float)>&& capacityChangeHandler) noexcept
{
    this->_capacityChangeHandler = capacityChangeHandler;
}

bool IOPowerSource::registerForCapacityChanges() noexcept
{
    if(this->_runLoopSource != nullptr)
    {
        os_log(DefaultLog, "Already registered for capacity changes.");
        return false;
    }
    
    const auto runLoopSource = IOPSNotificationCreateRunLoopSource(this->_batteryCapacityDidChange, (void*)this);
    CFRunLoopAddSource(CFRunLoopGetCurrent(), runLoopSource, kCFRunLoopDefaultMode);
    
    this->_runLoopSource = runLoopSource;
    
    return true;
}

bool IOPowerSource::unregisterFromCapacityChanges() noexcept
{
    if(this->_runLoopSource == nullptr)
    {
        os_log(DefaultLog, "Not registered for capacity changes.");
        return false;
    }
    
    CFRunLoopSourceRef source = static_cast<CFRunLoopSourceRef>(this->_runLoopSource);
    CFRunLoopRemoveSource(CFRunLoopGetCurrent(), source, kCFRunLoopDefaultMode);
    CFRelease(source);
    this->_runLoopSource = nullptr;
    
    return true;
}
