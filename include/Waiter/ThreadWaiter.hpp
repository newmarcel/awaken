//
//  ThreadWaiter.hpp
//  Awaken
//
//  Created by Marcel Dierkes on 25.04.20.
//  Copyright © 2020 Marcel Dierkes. All rights reserved.
//

#ifndef ThreadWaiter_hpp
#define ThreadWaiter_hpp

#include <chrono>
#include <functional>
#include <optional>
#include <thread>
#include "Waiter.hpp"

namespace Awaken
{

class ThreadWaiter : public Waiter
{
public:
    
#pragma mark - Life Cycle
    
    ThreadWaiter() noexcept = default;
    
#pragma mark - Properties
    
    void setTimeout(std::chrono::seconds) noexcept;
    void setTimeoutHandler(std::function<void()>&&) noexcept;
    
#pragma mark - Running
    
    bool isRunning() const noexcept;
    bool run() noexcept;
    bool cancel() noexcept;
    
private:
    std::chrono::seconds _timeout { 0 };
    std::optional<std::function<void()>> _timeoutHandler = std::nullopt;
    bool _running = false;
    std::thread _thread;
};

}

#endif /* ThreadWaiter_hpp */
