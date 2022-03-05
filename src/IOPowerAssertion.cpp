//
//  IOPowerAssertion.cpp
//  Awaken
//
//  Created by Marcel Dierkes on 25.04.20.
//  Copyright © 2020 Marcel Dierkes. All rights reserved.
//

#include <Awaken/IOPowerAssertion.hpp>
#include <CoreFoundation/CoreFoundation.h>
#include <IOKit/pwr_mgt/IOPMLib.h>
#include "Log.hpp"

using namespace std;
using namespace Awaken;


#pragma mark - Awaken::CoreFoundationString

namespace Awaken
{
class CoreFoundationString
{
public:
    CoreFoundationString(std::string stdString) noexcept
    {
        const auto data = reinterpret_cast<const UInt8*>(stdString.data());
        const auto size = static_cast<CFIndex>(stdString.size());
        this->_cfString = CFStringCreateWithBytes(kCFAllocatorDefault,
                                                  data, size,
                                                  kCFStringEncodingUTF8,
                                                  false);
    }
    
    ~CoreFoundationString() { CFRelease(this->_cfString); }
    
    CFStringRef get() const noexcept { return this->_cfString; }
    CFStringRef operator()() const noexcept { return this->_cfString; }
    
private:
    CFStringRef _cfString;
};
}

#pragma mark - Life Cycle

IOPowerAssertion::IOPowerAssertion() noexcept
    : timeout(0s)
    , preventUserIdleSystemSleep(false)
    , preventUserIdleDisplaySleep(false)
    , _systemAssertionID(nullopt)
    , _displayAssertionID(nullopt)
{
}

IOPowerAssertion::~IOPowerAssertion() noexcept
{
    this->cancel();
}

#pragma mark - Running

bool IOPowerAssertion::isRunning() const noexcept
{
    return this->_systemAssertionID != nullopt || this->_displayAssertionID != nullopt;
}

bool IOPowerAssertion::run() noexcept
{
    if(this->isRunning())
    {
        os_log(DefaultLog, "An assertion is already running: %{public}d/%{public}d.",
               this->_systemAssertionID.value_or(-1),
               this->_displayAssertionID.value_or(-1)
               );
        return false;
    }
    
    bool runResult = true;
    
    const auto timeout = this->timeout;
    if(timeout > 0s)
    {
        os_log(DefaultLog, "Asserting for %{public}lld seconds.", timeout.count());
    }
    else
    {
        os_log(DefaultLog, "Asserting indefinitely.");
    }
    
    const auto interval = static_cast<CFTimeInterval>(timeout.count());
    
    auto preventUserIdleSystemSleep = this->preventUserIdleSystemSleep;
    if(preventUserIdleSystemSleep == true)
    {
        os_log(DefaultLog, "Preventing user idle system sleep.");
        
        const auto reason = CFSTR("preventing user idle system sleep");
        IOPMAssertionID assertionID = 0;
        auto assertionName = CoreFoundationString(this->name);
        auto result = IOPMAssertionCreateWithDescription(kIOPMAssertionTypePreventUserIdleSystemSleep,
                                                         assertionName(),
                                                         reason, nullptr, nullptr,
                                                         interval,
                                                         kIOPMAssertionTimeoutActionRelease,
                                                         &assertionID);
        if(result == kIOReturnSuccess)
        {
            this->_systemAssertionID = assertionID;
        }
        else
        {
            os_log(DefaultLog, "Failed preventing user idle system sleep.");
            this->_systemAssertionID = nullopt;
            runResult = false;
        }
    }
    
    auto preventUserIdleDisplaySleep = this->preventUserIdleDisplaySleep;
    if(preventUserIdleDisplaySleep == true)
    {
        os_log(DefaultLog, "Preventing user idle display sleep.");
        
        const auto reason = CFSTR("preventing user idle system sleep");
        IOPMAssertionID assertionID = 0;
        auto assertionName = CoreFoundationString(this->name);
        auto result = IOPMAssertionCreateWithDescription(kIOPMAssertionTypePreventUserIdleDisplaySleep,
                                                         assertionName(),
                                                         reason, nullptr, nullptr,
                                                         interval,
                                                         kIOPMAssertionTimeoutActionRelease,
                                                         &assertionID);
        if(result == kIOReturnSuccess)
        {
            this->_displayAssertionID = assertionID;
        }
        else
        {
            os_log(DefaultLog, "Failed preventing user idle display sleep.");
            this->_displayAssertionID = nullopt;
            runResult = false;
        }
    }
    
    if(runResult == false)
    {
        if(auto assertionID = this->_systemAssertionID)
        {
            IOPMAssertionRelease(*assertionID);
        }
        if(auto assertionID = this->_displayAssertionID)
        {
            IOPMAssertionRelease(*assertionID);
        }
    }
    return runResult;
}

bool IOPowerAssertion::cancel() noexcept
{
    if(!this->isRunning())
    {
        os_log(DefaultLog, "Cannot cancel, no assertion is running.");
        return false;
    }
    
    if(auto assertionID = this->_systemAssertionID)
    {
        os_log(DefaultLog, "Cancel system sleep assertion.");
        IOPMAssertionRelease(*assertionID);
    }
    if(auto assertionID = this->_displayAssertionID)
    {
        os_log(DefaultLog, "Cancel display sleep assertion.");
        IOPMAssertionRelease(*assertionID);
    }
    
    return true;
}
