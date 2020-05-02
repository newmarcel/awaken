//
//  DispatchWaiter.hpp
//  Awaken
//
//  Created by Marcel Dierkes on 29.04.20.
//  Copyright © 2020 Marcel Dierkes. All rights reserved.
//

#ifndef DispatchWaiter_hpp
#define DispatchWaiter_hpp

#include <chrono>
#include <functional>
#include <optional>
#include <dispatch/dispatch.h>
#include "Waiter.hpp"

namespace Awaken
{

/*
 BROKEN. DON'T USE. YET.
 */
class DispatchWaiter : public Waiter
{
public:
    
#pragma mark - Life Cycle
    
    DispatchWaiter() noexcept;
    ~DispatchWaiter() noexcept;
    
    DispatchWaiter(const DispatchWaiter&) = delete;
    DispatchWaiter& operator=(const DispatchWaiter&) = delete;
    
    DispatchWaiter(DispatchWaiter&&) = delete;
    DispatchWaiter& operator=(DispatchWaiter&&) = delete;
    
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
    dispatch_queue_t _dispatchQueue;
};

}

#endif /* DispatchWaiter_hpp */
