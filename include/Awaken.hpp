//
//  Awaken.hpp
//  Awaken
//
//  Created by Marcel Dierkes on 25.04.20.
//  Copyright © 2020 Marcel Dierkes. All rights reserved.
//

#ifndef Awaken_hpp
#define Awaken_hpp

#include <chrono>
#include <memory>
#include <string>
#include <functional>

namespace Awaken
{
class IOPowerAssertion;
class IOPowerSource;
class Waiter;

/// Represents an infinite timeout duration
const std::chrono::seconds InfiniteTimeout { 0 };

class Awaken
{
public:
    
#pragma mark - Life Cycle
    
    /// The designated initializer
    /// @param name The current tool's name used in system logs
    Awaken(std::string name) noexcept;
    Awaken() noexcept;
    ~Awaken() noexcept;
    
    Awaken(const Awaken&) = delete;
    Awaken& operator=(const Awaken&) = delete;
    
    Awaken(Awaken&&) noexcept;
    Awaken& operator=(Awaken&&) noexcept;
    
#pragma mark - Properties
    
    /// Returns the library version
    static std::string version() noexcept;
    
    /// Returns the tool name used in system logs
    std::string name() const noexcept;
    
    /// Sets the timeout for the selected power assertions.
    /// @param timeout A timeout in seconds, if set to 0 or InfiniteTimeout,
    ///                it will be assumed to be an indefinite timeout.
    /// @returns true if the timeout could be modified.
    bool setTimeout(std::chrono::seconds timeout) noexcept;
    /// The timeout for the selected power assertions.
    /// A value of 0 (aka InfiniteTimeout) represents
    /// an indefinite timeout.
    std::chrono::seconds timeout() const noexcept;
    
    /// Sets an optional timeout handler that will be called
    /// on a private thread when the timeout is reached.
    void setTimeoutHandler(std::function<void()>&&) noexcept;
    
    /// Prevents the system from sleeping automatically
    /// due to a lack of user activity if set to true.
    /// @param value prevent system sleep when true
    /// @returns true if the value could be modified.
    bool setPreventUserIdleSystemSleep(bool value) noexcept;
    /// Prevents the system from sleeping automatically if true.
    bool preventUserIdleSystemSleep() const noexcept;
    
    /// Prevents the display from dimming automatically if set to true.
    /// @param value prevent display sleep when true
    /// @returns true if the value could be modified.
    bool setPreventUserIdleDisplaySleep(bool value) noexcept;
    /// Prevents the display from dimming automatically if true.
    bool preventUserIdleDisplaySleep() const noexcept;
    
#pragma mark - Running
    
    /// Returns true if a sleep assertion is currently running
    bool isRunning() const noexcept;
    
    /// Runs all configured sleep assertions and
    /// keeps the current process awake.
    /// @returns false if the sleep assertions cannot be created.
    bool run() noexcept;
    
    /// Cancels any sleep assertions.
    void cancel() noexcept;
    
private:
    std::unique_ptr<IOPowerAssertion> _powerAssertion;
    std::unique_ptr<IOPowerSource> _powerSource;
    std::unique_ptr<Waiter> _waiter;
};

}

#endif /* Awaken_hpp */
