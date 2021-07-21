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

/// @file
/// Main entry point of the Adaptive Autosar Package Manager.
/// This is the main.cpp file containing the main function of the Adaptive Autosar Package Manager.

#include "ara/log/logging.h"
#include <exception>
#include <cstdlib>
#include "package_management_application.h"

/// @brief The  main function forwards all operations
/// to the Execute method of ara::ucm::pkgmgr::PackageManagementApplication.
/// Important program steps and errors are logged through the ara::log API
/// @return Returns 0 if program execution was successful
int main()
{

    // InitLogging() call should be removed later
    // This function has been already removed from SWS Log & Trace
    ara::log::InitLogging("UCM",
        "UpdateAndConfigurationManagement",
        ara::log::LogLevel::kVerbose,
        ara::log::LogMode::kConsole | ara::log::LogMode::kRemote);

    ara::ucm::pkgmgr::PackageManagementApplication app;
    return app.Execute();
}