//
//  IOPowerSource.cpp
//  awaken
//
//  Created by Marcel Dierkes on 07.05.20.
//  Copyright © 2020 Marcel Dierkes. All rights reserved.
//

#include "IOPowerSource/IOPowerSource.hpp"
#include <CoreFoundation/CoreFoundation.h>
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
}


// Reference Sources for KYABatteryStatus.m

//const CGFloat KYABatteryStatusUnavailable = -1.0;
//
//static void KYABatteryStatusChangeHandler(void *context);
//
//@interface KYABatteryStatus ()
//@property (nonatomic, nullable) CFRunLoopSourceRef runLoopSource;
//@end
//
//@implementation KYABatteryStatus
//
//- (void)dealloc
//{
//    [self unregisterFromCapacityChanges];
//}
//
//- (BOOL)isBatteryStatusAvailable
//{
//    KYA_AUTO powerSourceInfos = [self powerSourceInfos];
//    if(powerSourceInfos == nil)
//    {
//        return NO;
//    }
//
//    KYA_AUTO key = [NSString stringWithUTF8String:kIOPSTypeKey];
//    KYA_AUTO internalBatteryTypeKey = [NSString stringWithUTF8String:kIOPSInternalBatteryType];
//    NSString *batteryType = powerSourceInfos[key];
//    return [batteryType isEqualToString:internalBatteryTypeKey];
//}
//
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
//
//- (nullable NSDictionary *)powerSourceInfos
//{
//    KYA_AUTO blob = IOPSCopyPowerSourcesInfo();
//    KYA_AUTO sourcesList = IOPSCopyPowerSourcesList(blob);
//    CFRelease(blob);
//
//    if(CFArrayGetCount(sourcesList) == 0) {
//        CFRelease(sourcesList);
//        return nil;
//    }
//
//    KYA_AUTO powerSource = IOPSGetPowerSourceDescription(blob, CFArrayGetValueAtIndex(sourcesList, 0));
//    CFRetain(powerSource);
//    CFRelease(sourcesList);
//
//    CFAutorelease(powerSource);
//    return (__bridge NSDictionary *)powerSource;
//}
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
