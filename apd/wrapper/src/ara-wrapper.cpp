// --------------------------------------------------------------------------
// |              _    _ _______     .----.      _____         _____        |
// |         /\  | |  | |__   __|  .  ____ .    / ____|  /\   |  __ \       |
// |        /  \ | |  | |  | |    .  / __ \ .  | (___   /  \  | |__) |      |
// |       / /\ \| |  | |  | |   .  / / / / v   \___ \ / /\ \ |  _  /       |
// |      / /__\ \ |__| |  | |   . / /_/ /  .   ____) / /__\ \| | \ \       |
// |     /________\____/   |_|   ^ \____/  .   |_____/________\_|  \_\      |
// |                              . _ _  .                                  |
// --------------------------------------------------------------------------
//
// All Rights Reserved.
// Any use of this source code is subject to a license agreement with the
// AUTOSAR development cooperation.
// More information is available at www.autosar.org.
//
// Disclaimer
//
// This work (specification and/or software implementation) and the material
// contained in it, as released by AUTOSAR, is for the purpose of information
// only. AUTOSAR and the companies that have contributed to it shall not be
// liable for any use of the work.
//
// The material contained in this work is protected by copyright and other
// types of intellectual property rights. The commercial exploitation of the
// material contained in this work requires a license to such intellectual
// property rights.
//
// This work may be utilized or reproduced without any modification, in any
// form or by any means, for informational purposes only. For any other
// purpose, no part of the work may be utilized or reproduced, in any form
// or by any means, without permission in writing from the publisher.
//
// The work has been developed for automotive applications only. It has
// neither been developed, nor tested for non-automotive applications.
//
// The word AUTOSAR and the AUTOSAR logo are registered trademarks.
// --------------------------------------------------------------------------

#include <signal.h>
#include <unistd.h>
#include <libgen.h>
#include <iostream>

#include "ara-wrapper.h"

namespace apd {
namespace wrapper {

AdaptiveWrapper::~AdaptiveWrapper()
{
}

void AdaptiveWrapper::Initialize()
{
    if(argsstr_.size() > 2) {
        // get the actual application name from the argument list. Skip wrapper name and manifest path
        app_name_ = argsstr_[2];
        for (auto it = argsstr_.begin() + 2; it != argsstr_.end(); ++it) {
            args_.emplace_back(const_cast<char*>(it->c_str()));
        }
        args_.emplace_back(nullptr);
    } else {
        std::cout << "Invalid arguments list defined for wrapper application:" << argsstr_.at(0) << "! Check application Manifest!";
    }
}

void AdaptiveWrapper::Run()
{
    if(!app_name_.empty()) {
        std::cout << "starting:" << app_name_.c_str();
        //execute the application executable with the required arguments
        execv(app_name_.c_str(), args_.data());

        //in normal conditions we shall never reach this point
        std::cout << "Failed executing application:" << app_name_ << "in wrapper application:" << argsstr_.at(0);
    } else {
        std::cout << "Invalid application name to execute in wrapper application:" << argsstr_.at(0);
    }
}

void AdaptiveWrapper::Shutdown()
{
}

} /* namespace wrapper */
} /* namespace apd */
