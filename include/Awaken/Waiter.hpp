//
//  Waiter.hpp
//  awaken
//
//  Created by Marcel Dierkes on 29.04.20.
//  Copyright © 2020 Marcel Dierkes. All rights reserved.
//

#ifndef Waiter_hpp
#define Waiter_hpp

#include <chrono>
#include <functional>

namespace Awaken
{

class Waiter
{
public:
    virtual ~Waiter() = default;
    
    virtual void setTimeout(std::chrono::seconds) noexcept = 0;
    virtual void setTimeoutHandler(std::function<void()>&&) noexcept = 0;
    
#pragma mark - Running
    
    virtual bool isRunning() const noexcept = 0;
    virtual bool run() noexcept = 0;
    virtual bool cancel() noexcept = 0;
};

}

#endif /* Waiter_hpp */
