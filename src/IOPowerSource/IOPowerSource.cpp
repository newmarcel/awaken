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
{
}

IOPowerSource::~IOPowerSource() noexcept
{
    //- (void)dealloc
    //{
    //    [self unregisterFromCapacityChanges];
    //}
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

#pragma mark - Properties

bool IOPowerSource::isBatteryStatusAvailable() const noexcept
{
    if(auto powerSourceDescription = CopyPowerSourceDescription())
    {
        const auto key = kIOPSTypeKey;
        const auto internalBatteryTypeKey = kIOPSInternalBatteryType;
        
        auto batteryType = CFDictionaryGetValue(*powerSourceDescription, key);
        
        const auto isInternalBattery = CFEqual(batteryType, internalBatteryTypeKey);
        CFRelease(*powerSourceDescription);
        
        return isInternalBattery;
    }
    else
    {
        return false;
    }
}

float IOPowerSource::batteryCapacity() const noexcept
{
    abort();
    
    //- (CGFloat)currentCapacity
    //{
    //    KYA_AUTO powerSourceInfos = [self powerSourceInfos];
    //    if(powerSourceInfos == nil)
    //    {
    //        return KYABatteryStatusUnavailable;
    //    }
    //
    //    KYA_AUTO key = [NSString stringWithUTF8String:kIOPSCurrentCapacityKey];
    //    NSNumber *capacity = powerSourceInfos[key];
    //    if(capacity == nil)
    //    {
    //        return KYABatteryStatusUnavailable;
    //    }
    //
    //    return capacity.floatValue;
    //}
}

#pragma mark - Capacity Changes

// Reference Sources for KYABatteryStatus.m

//static void KYABatteryStatusChangeHandler(void *context);
//
//@interface KYABatteryStatus ()
//@property (nonatomic, nullable) CFRunLoopSourceRef runLoopSource;
//@end
//
//@implementation KYABatteryStatus
//

//

//

//
//
//#pragma mark - Capacity Change Handler
//
//- (void)registerForCapacityChangesIfNeeded
//{
//    if(self.runLoopSource)
//    {
//        return;
//    }
//
//    KYA_AUTO runLoopSource = IOPSNotificationCreateRunLoopSource(KYABatteryStatusChangeHandler, (__bridge void *)self);
//    CFRunLoopAddSource(CFRunLoopGetCurrent(), runLoopSource, kCFRunLoopDefaultMode);
//
//    self.runLoopSource = runLoopSource;
//    CFRelease(runLoopSource);
//}
//
//- (void)unregisterFromCapacityChanges
//{
//    if(!self.runLoopSource)
//    {
//        return;
//    }
//
//    CFRunLoopRemoveSource(CFRunLoopGetCurrent(), self.runLoopSource, kCFRunLoopDefaultMode);
//    self.runLoopSource = nil;
//}
//
//@end
//
//#pragma mark - KYABatteryStatusChangeHandler
//
//static void KYABatteryStatusChangeHandler(void *context)
//{
//    KYA_AUTO batteryStatus = (__bridge KYABatteryStatus *)context;
//    if(batteryStatus.capacityChangeHandler)
//    {
//        batteryStatus.capacityChangeHandler(batteryStatus.currentCapacity);
//    }
//}
