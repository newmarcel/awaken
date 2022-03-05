//
//  ThreadWaiter.cpp
//  Awaken
//
//  Created by Marcel Dierkes on 25.04.20.
//  Copyright © 2020 Marcel Dierkes. All rights reserved.
//

#include <Awaken/ThreadWaiter.hpp>
#include "../Log.hpp"

using namespace std;
using namespace Awaken;

#pragma mark - Life Cycle

#pragma mark - Properties

void ThreadWaiter::setTimeout(std::chrono::seconds timeout) noexcept
{
    this->_timeout = timeout;
}

void ThreadWaiter::setTimeoutHandler(std::function<void()>&& timeoutHandler) noexcept
{
    this->_timeoutHandler = timeoutHandler;
}

#pragma mark - Running

bool ThreadWaiter::isRunning() const noexcept
{
    return this->_running;
}

bool ThreadWaiter::run() noexcept
{
    if(this->isRunning())
    {
        os_log(DefaultLog, "A waiter is already running.");
        return false;
    }
    
    this->_running = true;
    
    const auto timeout = this->_timeout;
    const auto timeoutHandler = this->_timeoutHandler;
    
    if(timeout == 0s)
    {
        this->_thread = thread([timeoutHandler, this]{
            os_log(DefaultLog, "Waiting indefinitely…");
            
            do {
                this_thread::sleep_for(1s);
                this_thread::yield();
            } while(this->_running);
            
            os_log(DefaultLog, "Cancelled.");
            
            if(timeoutHandler != nullopt)
            {
                (*timeoutHandler)();
            }
        });
    }
    else
    {
        this->_thread = thread([timeout, timeoutHandler, this]{
            os_log(DefaultLog, "Waiting for %{public}lld seconds.", timeout.count());
            
            const auto start = chrono::system_clock::now();
            const auto end = start + timeout;
            
            do {
                this_thread::sleep_for(1s);
                this_thread::yield();
            } while(chrono::system_clock::now() < end && this->_running);
            
            os_log(DefaultLog, "Waited.");
            
            if(timeoutHandler != nullopt)
            {
                (*timeoutHandler)();
            }
        });
    }
    
    return true;
}

bool ThreadWaiter::cancel() noexcept
{
    this->_running = false;
    os_log(DefaultLog, "Cancel waiter.");
    return true;
}
