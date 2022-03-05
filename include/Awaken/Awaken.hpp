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
    
    /// @name Power Assertions
    /// Configures the desired power assertions to
    /// actually keep the system or display awake.
    /// @{
    
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
    
    /// @}
    
#pragma mark - Timeout
    
    /// @name Timeout
    /// Configures the duration of the configured power assertions
    /// @{
    
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
    
    /// @}
    
#pragma mark - Minimum Battery Capacity
    
    /// @name Minimum Battery Capacity
    /// Configures a minimum battery capacity threshold
    /// until power assertions are held.
    /// @note These properties only apply if the system
    /// has a built-in battery.
    /// @{
    
    /// Returns true if the current device has a built-in battery.
    bool hasBattery() const noexcept;
    
    /// Set the minimum limit for the battery capacity,
    /// if this capacity is reached, the sleep assertions
    /// will be released.
    /// @param capacity The battery capacity (e.g. 20.0f for 20 % capacity)
    void setMinimumBatteryCapacity(float capacity) noexcept;
    
    /// Returns the minimum battery capacity limit. If the
    /// current devices reaches this limit, the sleep assertions
    /// will be released. (e.g. 20.0f for 20 % capacity)
    float minimumBatteryCapacity() const noexcept;
    
    /// An optional handler that will be called when the battery
    /// capacity reaches the `minimumBatteryCapacity()`.
    /// Any sleep assertions will be cancelled when the minimum
    /// battery capacity is reached.
    void setMinimumBatteryCapacityReachedHandler(std::function<void(float)>&&) noexcept;
    
    /// @}
    
#pragma mark - Running
    
    /// @name Running
    /// Runs and cancels all configured power assertions.
    /// @{
    
    /// Returns true if a sleep assertion is currently running
    bool isRunning() const noexcept;
    
    /// Runs all configured sleep assertions and
    /// keeps the current process awake.
    /// @returns false if the sleep assertions cannot be created.
    bool run() noexcept;
    
    /// Cancels any sleep assertions.
    void cancel() noexcept;
    
    /// @}
    
private:
    std::unique_ptr<IOPowerAssertion> _powerAssertion;
    std::unique_ptr<IOPowerSource> _powerSource;
    std::unique_ptr<Waiter> _waiter;
    float _minimumBatteryCapacity;
};

}

#endif /* Awaken_hpp */
