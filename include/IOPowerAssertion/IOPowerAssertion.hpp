//
//  IOPowerAssertion.hpp
//  Awaken
//
//  Created by Marcel Dierkes on 25.04.20.
//  Copyright © 2020 Marcel Dierkes. All rights reserved.
//

#ifndef PowerAssertion_hpp
#define PowerAssertion_hpp

#include <string>
#include <chrono>
#include <optional>

namespace Awaken
{

class IOPowerAssertion
{
public:
    
#pragma mark - Properties
    
    std::string name { "Awaken" };
    std::chrono::seconds timeout { 0 };
    bool preventUserIdleSystemSleep = false;
    bool preventUserIdleDisplaySleep = false;
    
#pragma mark - Life Cycle
    
    IOPowerAssertion() noexcept;
    ~IOPowerAssertion() noexcept;
    
    IOPowerAssertion(const IOPowerAssertion&) = delete;
    IOPowerAssertion& operator=(const IOPowerAssertion&) = delete;
    
    IOPowerAssertion(IOPowerAssertion&&) = default;
    IOPowerAssertion& operator=(IOPowerAssertion&&) = default;
    
#pragma mark - Running
    
    bool isRunning() const noexcept;
    bool run() noexcept;
    bool cancel() noexcept;
    
private:
    std::optional<uint32_t> _systemAssertionID;
    std::optional<uint32_t> _displayAssertionID;
};

}

#endif /* PowerAssertion_hpp */
