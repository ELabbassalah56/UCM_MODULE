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

#ifndef ARA_UCM_PKGMGR_STORAGE_EXECUTION_MANAGER_STUB_H_
#define ARA_UCM_PKGMGR_STORAGE_EXECUTION_MANAGER_STUB_H_

#include "parsing/application.h"

namespace ara
{
namespace ucm
{
namespace pkgmgr
{

/// @brief Stub for ExecutionManager interface which does not exist yet.
/// This stub is used to document the places in code where an interface to the execution manager is needed.
class ExecutionManagerStub
{
public:
    /// @brief Execution manager mock
    ///
    /// @note A placeholder, since an interface to the execution manager does not exist yet
    ExecutionManagerStub();

    /// @brief Lets the execution manager restart an application
    ///
    /// @note A placeholder, since an interface to the execution manager does not exist yet
    ///
    /// @param app The application to restart
    void RestartApplication(const Application& app);

    /// @brief Lets the execution manager start an application
    ///
    /// @note A placeholder, since an interface to the execution manager does not exist yet
    ///
    /// @param app The application to start
    void StartApplication(const Application& app);

    /// @brief Lets the execution manager stop an application
    ///
    /// @note A placeholder, since an interface to the execution manager does not exist yet
    ///
    /// @param app The application to stop
    void StopApplication(const Application& app);
};

}  // namespace pkgmgr
}  // namespace ucm
}  // namespace ara

#endif  // ARA_UCM_PKGMGR_STORAGE_EXECUTION_MANAGER_MOCK_H_
