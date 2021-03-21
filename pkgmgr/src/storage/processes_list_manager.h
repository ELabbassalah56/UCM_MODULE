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

#ifndef ARA_UCM_PKGMGR_STORAGE_PROCESSESLISTMANAGER_H_
#define ARA_UCM_PKGMGR_STORAGE_PROCESSESLISTMANAGER_H_

#include "ara/core/string.h"
#include "ara/core/vector.h"
#include "ara/log/logging.h"
#include "filesystem_swcl_manager.h"

namespace ara
{
namespace ucm
{
namespace pkgmgr
{

/// @brief processes list manager working with the filesystem directly
class ProcessesListManager
{
public:
    explicit ProcessesListManager(ara::core::String rootDirectory);

    /// @brief Updates the processes list with current existing applications
    void UpdateProcessesList(ara::core::Vector<SoftwareCluster> activeSWCLs);

private:
    /// @brief Processes list name
    const ara::core::String kProcessesListFileName = "/PROCESSES.json";

    /// @brief Key name within the processes list
    const ara::core::String kKeyName = "processes";

    /// @brief Holds the application prefixes (e.x /opt/pkgmgr_sample)
    ara::core::Vector<ara::core::String> appsPaths;

    /// @brief Holds UCM update directory
    ara::core::String rootDirectory_;

    /// @brief Logger
    ara::log::Logger& log{ara::log::CreateLogger("PKPL", "PackageManager-ProcessesListManager context")};
};

}  // namespace pkgmgr
}  // namespace ucm
}  // namespace ara

#endif  // ARA_UCM_PKGMGR_STORAGE_PROCESSESLISTMANAGER_H_
