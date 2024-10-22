//
//  main.cpp
//  Awaken
//
//  Created by Marcel Dierkes on 24.12.19.
//  Copyright © 2019 Marcel Dierkes. All rights reserved.
//

#include <stdlib.h>
#include <print>
#include <chrono>
#include <dispatch/dispatch.h>
#include <Awaken/Awaken.hpp>
#include <cxxopts.hpp>

void RunAwaken(std::chrono::seconds timeout, bool preventDisplaySleep, bool preventSystemSleep, std::optional<float> minimumBatteryCapacity)
{
//    __block
    auto awaken = Awaken::Awaken("awaken command-line tool");
    awaken.setPreventUserIdleSystemSleep(preventSystemSleep);
    awaken.setPreventUserIdleDisplaySleep(preventDisplaySleep);
    
    using namespace std::chrono_literals;
    awaken.setTimeout(timeout);
    
    if(const auto capacity = minimumBatteryCapacity)
    {
        awaken.setMinimumBatteryCapacity(*minimumBatteryCapacity);
        awaken.setMinimumBatteryCapacityReachedHandler([](float capacity) {
            std::println("Minimum battery capacity reached {:.0f}", capacity);
        });
    }
    
    awaken.setTimeoutHandler([]{
        exit(EXIT_SUCCESS);
    });
    awaken.run();
    
//    dispatch_after(dispatch_time(DISPATCH_TIME_NOW, (int64_t)(5.0f * NSEC_PER_SEC)), dispatch_get_main_queue(), ^{
//        awaken.cancel();
//    });
    
// ------------
    
    dispatch_main();
}

cxxopts::ParseResult ParseArguments(int argc, char* argv[])
{
    try
    {
        cxxopts::Options options { argv[0], "awaken - prevents your Mac from going to sleep." };
        
        options.add_options()
        ("d,display-sleep", "prevent the display from idle sleeping", cxxopts::value<bool>()->default_value("false"))
        ("s,system-sleep", "prevent the system from idle sleeping", cxxopts::value<bool>()->default_value("true"))
        ("t,timeout", "timeout in seconds until the sleep assertion expires", cxxopts::value<int64_t>()->default_value("0"), "N")
        ("b,battery-level", "a minimum battery level on devices with a built-in battery that causes the sleep assertion to expire (e.g. 20 for <= 20% remaining battery). Values above 95 are unreliable and depend on the battery health.", cxxopts::value<uint8_t>()->default_value("0"), "N")
        ;
        
        options.add_options("Help")
        ("h,help", "print this help")
        ("v,version", "print version information")
        ;
        
        const auto result = options.parse(argc, argv);
        
        if(result.count("version"))
        {
            std::println("Awaken {}", Awaken::Awaken::version());
            exit(EXIT_SUCCESS);
        }
        
        if(result.count("help"))
        {
            std::println("{}", options.help());
            exit(EXIT_SUCCESS);
        }
        
        return result;
    }
    catch (const cxxopts::exceptions::exception& e)
    {
        std::println("Failed parsing options: {}", e.what());
        exit(EXIT_FAILURE);
    }
}

int main(int argc, char **argv)
{
    const auto result = ParseArguments(argc, argv);
    
    std::chrono::seconds timeout { 0 };
    bool preventDisplaySleep = false;
    bool preventSystemSleep = false;
    std::optional<float> minimumBatteryCapacity = std::nullopt;
    
    if(result.count("display-sleep"))
    {
        preventDisplaySleep = true;
    }
    
    if(result.count("system-sleep"))
    {
        preventSystemSleep = true;
    }
    
    if(!result.count("display-sleep") && !result.count("system-sleep"))
    {
        // If no parameters are given, prevent system sleep
        preventSystemSleep = true;
    }
    
    if(result.count("timeout"))
    {
        auto customTimeout = result["timeout"].as<int64_t>();
        timeout = std::chrono::seconds { customTimeout };
    }
    
    if(result.count("battery-level"))
    {
        auto batteryLevel = result["battery-level"].as<uint8_t>();
        if(batteryLevel > 100)
        {
            std::println("Unsupported battery percentage '{}' provided.", batteryLevel);
            exit(EXIT_FAILURE);
        }
        minimumBatteryCapacity = static_cast<float>(batteryLevel);
    }
    
    RunAwaken(timeout, preventDisplaySleep, preventSystemSleep, minimumBatteryCapacity);
    
    return EXIT_SUCCESS;
}
