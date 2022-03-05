//
//  Awaken.cpp
//  Awaken
//
//  Created by Marcel Dierkes on 25.04.20.
//  Copyright © 2020 Marcel Dierkes. All rights reserved.
//

#include <Awaken/Awaken.hpp>
#include <Awaken/IOPowerAssertion.hpp>
#include <Awaken/IOPowerSource.hpp>
#include <Awaken/Waiter.hpp>
#include "Log.hpp"

#if __has_include("config.h")
#include <Awaken/config.h>
#else
#define AWAKEN_VERSION "current"
#endif

#define USE_DISPATCH_WAITER 0
#if USE_DISPATCH_WAITER
#include <Awaken/DispatchWaiter.hpp>
namespace Awaken { using WaiterClass = DispatchWaiter; }
#else
#include <Awaken/ThreadWaiter.hpp>
namespace Awaken { using WaiterClass = ThreadWaiter; }
#endif

using namespace std;
using ::Awaken::DefaultLog;

#pragma mark - Life Cycle

Awaken::Awaken::Awaken(string name) noexcept
    : _powerAssertion(make_unique<IOPowerAssertion>())
    , _powerSource(make_unique<IOPowerSource>())
    , _waiter(make_unique<WaiterClass>())
{
    this->_powerAssertion->name = name;
}

Awaken::Awaken::Awaken() noexcept : Awaken::Awaken::Awaken("Awaken") {};

Awaken::Awaken::~Awaken() noexcept = default;

Awaken::Awaken::Awaken(Awaken&& other) noexcept
    : _powerAssertion(move(other._powerAssertion))
    , _powerSource(move(other._powerSource))
    , _waiter(move(other._waiter))
{
}

Awaken::Awaken& Awaken::Awaken::operator=(Awaken&& other) noexcept = default;

#pragma mark - Properties

string Awaken::Awaken::version() noexcept
{
    return AWAKEN_VERSION;
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

#pragma mark - Timeout

bool Awaken::Awaken::setTimeout(chrono::seconds timeout) noexcept
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

chrono::seconds Awaken::Awaken::timeout() const noexcept
{
    return this->_powerAssertion->timeout;
}

void Awaken::Awaken::setTimeoutHandler(function<void()>&& timeoutHandler) noexcept
{
    this->_waiter->setTimeoutHandler(move(timeoutHandler));
}

#pragma mark - Minimum Battery Capacity

bool Awaken::Awaken::hasBattery() const noexcept
{
    return this->_powerSource->hasBattery();
}

void Awaken::Awaken::setMinimumBatteryCapacity(float capacity) noexcept
{
    os_log(DefaultLog, "Setting minimum battery capacity to %{public}.00f…", capacity);
    this->_minimumBatteryCapacity = capacity;
}

float Awaken::Awaken::minimumBatteryCapacity() const noexcept
{
    return this->_minimumBatteryCapacity;
}

void Awaken::Awaken::setMinimumBatteryCapacityReachedHandler(std::function<void(float)>&& handler) noexcept
{
    if(handler != nullptr)
    {
        if(!this->_powerSource->hasBattery())
        {
            os_log(DefaultLog, "Current device does not support a minimum battery capacity.");
            return;
        }
        
        const float currentMinimumCapacity = this->_minimumBatteryCapacity;
        this->_powerSource->setCapacityChangeHandler([handler, currentMinimumCapacity, this](float capacity) {
            if(capacity <= currentMinimumCapacity)
            {
                os_log(DefaultLog, "Minimum battery capacity reached: %{public}.00f", capacity);
                handler(capacity);
                this->cancel();
            }
        });
        this->_powerSource->registerForCapacityChanges();
    }
    else
    {
        this->_powerSource->unregisterFromCapacityChanges();
        this->_powerSource->setCapacityChangeHandler(nullptr);
    }
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
    this->_powerSource->setCapacityChangeHandler(nullptr);
}
