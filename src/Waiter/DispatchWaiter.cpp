//
//  DispatchWaiter.cpp
//  Awaken
//
//  Created by Marcel Dierkes on 29.04.20.
//  Copyright © 2020 Marcel Dierkes. All rights reserved.
//

#include <Awaken/DispatchWaiter.hpp>
#include "../Log.hpp"

using namespace std;
using namespace Awaken;

#pragma mark - Life Cycle

DispatchWaiter::DispatchWaiter() noexcept
    : _dispatchQueue(dispatch_queue_create("info.marcel-dierkes.Awaken.DispatchQueue", DISPATCH_QUEUE_SERIAL))
{
}

DispatchWaiter::~DispatchWaiter() noexcept
{
    dispatch_release(this->_dispatchQueue);
}

#pragma mark - Properties

void DispatchWaiter::setTimeout(std::chrono::seconds timeout) noexcept
{
    this->_timeout = timeout;
}

void DispatchWaiter::setTimeoutHandler(std::function<void()>&& timeoutHandler) noexcept
{
    this->_timeoutHandler = timeoutHandler;
}

#pragma mark - Running

bool DispatchWaiter::isRunning() const noexcept
{
    return this->_running;
}

bool DispatchWaiter::run() noexcept
{
    if(this->isRunning())
    {
        os_log(DefaultLog, "A waiter is already running.");
        return false;
    }
    
    this->_running = true;
    
    const auto nanoTimeout = chrono::duration_cast<chrono::nanoseconds>(this->_timeout);
    const auto timeoutHandler = this->_timeoutHandler;
    
    auto lambda = [this, timeoutHandler] {
        if(!this->_running) { return; }
        
        os_log(DefaultLog, "Waiting.");
        
        if(const auto& handler = timeoutHandler) {
            (*handler)();
        }
        this->_running = false;
        
        os_log(DefaultLog, "Waited.");
    };
    
    dispatch_after(dispatch_time(DISPATCH_TIME_NOW, nanoTimeout.count()), this->_dispatchQueue, ^{
        lambda();
    });
    
    return false;
}

bool DispatchWaiter::cancel() noexcept
{
    this->_running = false;
    os_log(DefaultLog, "Cancel waiter.");
    
    return true;
}
