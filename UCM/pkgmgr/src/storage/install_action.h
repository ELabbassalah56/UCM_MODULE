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

#ifndef ARA_UCM_PKGMGR_STORAGE_INSTALL_ACTION_H_
#define ARA_UCM_PKGMGR_STORAGE_INSTALL_ACTION_H_

#include "storage/reversible_action.h"
#include "parsing/software_package.h"
#include "filesystem.h"
#include "filesystem_swcl_manager.h"

namespace ara
{
namespace ucm
{
namespace pkgmgr
{

/// @brief Action for installation of a software package.
///
/// @uptrace{SWS_UCM_00001}
class InstallAction : public ReversibleAction
{
    using ReversibleAction::ReversibleAction;

public:
    /// @brief Executes the installation of a software package.
    ///
    /// The content of the software package @p package is temporarily installed in a directory
    /// until activation. If execution fails, no files are installed.
    ///
    /// @copydetails ReversibleAction::Execute()
    Result<void, ErrorCode> Execute() override;

    /// @brief Revert changes in installation directory created by the action.
    ///
    /// The installed software cluster is removed again.
    ///
    /// @copydetails ReversibleAction::RevertChanges()
    Result<void, ErrorCode> RevertChanges() override;

    /// @brief Commit changes in installation directory created by the action.
    ///
    /// The installed software cluster is persisted.
    ///
    /// @copydetails ReversibleAction::CommitChanges()
    Result<void, ErrorCode> CommitChanges() override;
};

}  // namespace pkgmgr
}  // namespace ucm
}  // namespace ara

#endif  // ARA_UCM_PKGMGR_STORAGE_INSTALL_ACTION_H_
