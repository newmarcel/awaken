//
//  Awaken.cpp
//  Awaken
//
//  Created by Marcel Dierkes on 25.04.20.
//  Copyright © 2020 Marcel Dierkes. All rights reserved.
//

#include "Awaken.hpp"
#include "IOPowerAssertion/IOPowerAssertion.hpp"
#include "Waiter/Waiter.hpp"
#include "Log.hpp"
#include "config.h"

#define USE_DISPATCH_WAITER 0
#if USE_DISPATCH_WAITER
#include "Waiter/DispatchWaiter.hpp"
namespace Awaken
{
using WaiterClass = DispatchWaiter;
}
#else
#include "Waiter/ThreadWaiter.hpp"
namespace Awaken
{
using WaiterClass = ThreadWaiter;
}
#endif

using namespace std;
using ::Awaken::DefaultLog;

#pragma mark - Life Cycle

Awaken::Awaken::Awaken(std::string name) noexcept
    : _powerAssertion(make_unique<IOPowerAssertion>())
    , _waiter(make_unique<WaiterClass>())
{
    this->_powerAssertion->name = name;
}

Awaken::Awaken::Awaken() noexcept : Awaken::Awaken::Awaken("Awaken") {};

Awaken::Awaken::~Awaken() noexcept = default;

Awaken::Awaken::Awaken(Awaken&& other) noexcept
    : _powerAssertion(move(other._powerAssertion))
    , _waiter(move(other._waiter))
{
}

Awaken::Awaken& Awaken::Awaken::operator=(Awaken&& other) noexcept = default;

#pragma mark - Properties

std::string Awaken::Awaken::version() noexcept
{
    return AWAKEN_VERSION;
}

bool Awaken::Awaken::setTimeout(std::chrono::seconds timeout) noexcept
{
    if(this->isRunning())
    {
        os_log(DefaultLog, "The timeout cannot be modified while running.");
        return false;
    }
    
    this->_waiter->setTimeout(timeout);
    this->_powerAssertion->timeout = move(timeout);
    
    return true;
}

std::chrono::seconds Awaken::Awaken::timeout() const noexcept
{
    return this->_powerAssertion->timeout;
}

void Awaken::Awaken::setTimeoutHandler(std::function<void()>&& timeoutHandler) noexcept
{
    this->_waiter->setTimeoutHandler(move(timeoutHandler));
}

bool Awaken::Awaken::setPreventUserIdleSystemSleep(bool preventUserIdleSystemSleep) noexcept
{
    if(this->isRunning())
    {
        os_log(DefaultLog, "The idle system sleep value cannot be modified while running.");
        return false;
    }
    
    this->_powerAssertion->preventUserIdleSystemSleep = move(preventUserIdleSystemSleep);
    
    return true;
}

bool Awaken::Awaken::preventUserIdleSystemSleep() const noexcept
{
    return this->_powerAssertion->preventUserIdleSystemSleep;
}

bool Awaken::Awaken::setPreventUserIdleDisplaySleep(bool preventUserIdleDisplaySleep) noexcept
{
    if(this->isRunning())
    {
        os_log(DefaultLog, "The idle display sleep value cannot be modified while running.");
        return false;
    }
    
    this->_powerAssertion->preventUserIdleDisplaySleep = move(preventUserIdleDisplaySleep);
    
    return true;
}

bool Awaken::Awaken::preventUserIdleDisplaySleep() const noexcept
{
    return this->_powerAssertion->preventUserIdleDisplaySleep;
}

#pragma mark - Running

bool Awaken::Awaken::isRunning() const noexcept
{
    return this->_powerAssertion->isRunning();
}

bool Awaken::Awaken::run() noexcept
{
    if(!this->_waiter->run())
    {
        os_log(DefaultLog, "Failed to wait for power assertion.");
        return false;
    }
    if(!this->_powerAssertion->run())
    {
        os_log(DefaultLog, "Failed to run power assertion.");
        this->_waiter->cancel();
        return false;
    }
    return true;
}

void Awaken::Awaken::cancel() noexcept
{
    if(!this->_waiter->cancel())
    {
        os_log(DefaultLog, "Failed to cancel power assertion waiter.");
    }
    if(!this->_powerAssertion->cancel())
    {
        os_log(DefaultLog, "Failed to cancel power assertion.");
    }
}
