//
//  Configuration.hpp
//  Awaken
//
//  Created by Marcel Dierkes on 24.12.19.
//  Copyright © 2019 Marcel Dierkes. All rights reserved.
//

#include <stdlib.h>
#include <iostream>
#include <chrono>
#include <dispatch/dispatch.h>
#include "Awaken.hpp"
#include "cxxopts/cxxopts.hpp"

void runAwaken(std::chrono::seconds timeout, bool preventDisplaySleep, bool preventSystemSleep)
{
//    __block
    auto awaken = Awaken::Awaken("Awaken CLI");
    awaken.setPreventUserIdleSystemSleep(preventSystemSleep);
    awaken.setPreventUserIdleDisplaySleep(preventDisplaySleep);
    
    using namespace std::chrono_literals;
    awaken.setTimeout(timeout);
    
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

cxxopts::ParseResult parseArguments(int argc, char* argv[])
{
    try
    {
        cxxopts::Options options { argv[0], "awaken - prevents your Mac from going to sleep." };
        
        options.add_options()
        ("d,display-sleep", "prevent the display from idle sleeping", cxxopts::value<bool>()->default_value("false"))
        ("s,system-sleep", "prevent the system from idle sleeping", cxxopts::value<bool>()->default_value("true"))
        ("t,timeout", "timeout in seconds until the sleep assertion expires", cxxopts::value<int64_t>()->default_value("0"), "N")
        ;
        
        options.add_options("Help") ("h,help", "print this help");
        
        const auto result = options.parse(argc, argv);
        
        if(result.count("help"))
        {
            std::cout << options.help() << std::endl;
            exit(EXIT_SUCCESS);
        }
        
        return result;
    }
    catch (const cxxopts::OptionException& e)
    {
        std::cout << "Failed parsing options: " << e.what() << std::endl;
        exit(EXIT_FAILURE);
    }
}

int main(int argc, char **argv)
{
    const auto result = parseArguments(argc, argv);
    
    std::chrono::seconds timeout { 0 };
    bool preventDisplaySleep = false;
    bool preventSystemSleep = false;
    
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
    
    runAwaken(timeout, preventDisplaySleep, preventSystemSleep);
    
    return EXIT_SUCCESS;
}
