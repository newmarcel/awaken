//
//  Log.hpp
//  Awaken
//
//  Created by Marcel Dierkes on 25.04.20.
//  Copyright © 2020 Marcel Dierkes. All rights reserved.
//

#ifndef Log_hpp
#define Log_hpp

#include <os/log.h>

namespace Awaken
{
const auto DefaultLog = os_log_create("info.marcel-dierkes.Awaken", "Awaken");
}

#endif /* Log_hpp */
