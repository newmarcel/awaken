//
//  IOPowerSource.hpp
//  awaken
//
//  Created by Marcel Dierkes on 07.05.20.
//  Copyright © 2020 Marcel Dierkes. All rights reserved.
//

#ifndef IOPowerSource_hpp
#define IOPowerSource_hpp

#include <functional>
#include <optional>

namespace Awaken
{

/// Represents the device power source with a battery capacity
/// if available.
class IOPowerSource
{
public:
    
    /// A return value representing the unavailability
    /// of a power source capacity.
    constexpr static float CapacityUnavailable = -1.0;
    
#pragma mark - Life Cycle
    
    IOPowerSource() noexcept;
    ~IOPowerSource() noexcept;
    
    IOPowerSource(const IOPowerSource&) = delete;
    IOPowerSource& operator=(const IOPowerSource&) = delete;
    
    IOPowerSource(IOPowerSource&&) = default;
    IOPowerSource& operator=(IOPowerSource&&) = default;
    
#pragma mark - Battery Capacity
    
    /// Returns true if the current device has a built-in battery.
    bool hasBattery() const noexcept;
    
    /// Returns the power source battery capacity or
    /// `CapacityUnavailable` if no capacity is available.
    float capacity() const noexcept;
    
#pragma mark - Capacity Changes
    
    /// An optional handler that will be called when the power source capacity
    /// changes and `registerForCapacityChanges()` was called.
    void setCapacityChangeHandler(std::function<void(float)>&&) noexcept;
    
    /// Registers the instance to receive power source capacity change
    /// events using the handler set with `setCapacityChangeHandler()`.
    /// @returns false if already registered for capacity changes
    bool registerForCapacityChanges() noexcept;
    
    /// Unregisters the instance from receiving power source capacity
    /// change events.
    /// @returns false if not registered for capacity changes
    bool unregisterFromCapacityChanges() noexcept;
    
private:
    float _capacity;
    std::optional<std::function<void(float)>> _capacityChangeHandler;
    void* _dispatchQueue;
    int _notificationToken;
};

}

#endif /* IOPowerSource_hpp */
