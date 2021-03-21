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

#ifndef ARA_UCM_PKGMGR_STORAGE_BASE_ACTION_H_
#define ARA_UCM_PKGMGR_STORAGE_BASE_ACTION_H_

#include <boost/filesystem.hpp>
#include "ara/core/string.h"
#include "parsing/software_package.h"
#include "filesystem.h"
#include "error_domain_ucmerrordomain.h"

namespace ara
{
namespace ucm
{
namespace pkgmgr
{

using ara::core::Result;
using ara::core::ErrorCode;
/// @brief Base class for all swcl actions.
///
/// An action may perform either installation, update or uninstall of a software package.
/// Subclasses of ReversibleAction implement actions
/// by moving files on a filesystem within a single partition.
/// Regardless of the result of the execution the action can be undone
///
/// @uptrace{SWS_UCM_00001}
/// @uptrace{SWS_UCM_00020}
/// @uptrace{SWS_UCM_00024}
class ReversibleAction
{
public:
    ReversibleAction() = delete;

    /// @brief Common constructor for all swcl-based actions.
    ///
    /// @param package The input package to the action
    /// @param destinationPath The location, where the package is supposed to be located
    ReversibleAction(const ara::core::String& destinationPath, const SoftwarePackage& package)
        : package_(package)
        , finalDestinationPath_(destinationPath)
    {}

    virtual ~ReversibleAction() = default;

    /// @brief Execute the action defined by the provided package.
    ///
    /// @returns no value if execution was successful, else see UCMErrorDomainErrc
    virtual Result<void, ErrorCode> Execute() = 0;

    /// @brief Revert modifications and cleanup temporary or obsolete artifacts created by the action.
    ///
    /// @returns no value if cleanup was successful, else see UCMErrorDomainErrc
    virtual Result<void, ErrorCode> RevertChanges() = 0;

    /// @brief Commit modifications and cleanup temporary or obsolete artifacts created by the action.
    ///
    /// @returns no value if cleanup was successful, else see UCMErrorDomainErrc
    virtual Result<void, ErrorCode> CommitChanges() = 0;

protected:
    Filesystem fs_;

    /// @brief Reference to the input package used in execute step
    const SoftwarePackage package_;

    /// @brief The final, not temporary location of the SWCL, a full path to the directory INCLUDING a folder with the
    /// name of the SWCL and a subfolder with its version.
    ///
    /// A path like "/A/B/C/SWCL1/1.0" where the "SWCL1" is a name of the SWCL, "1.0" is the version of the SWCL and the
    /// "/A/B/C/" part is common for allSWCLs.
    ara::core::String finalDestinationPath_ = "";

    /// @brief Logger for logging errors
    ara::log::Logger& log_{ara::log::CreateLogger("PKGM", "PackageManagerImpl context")};

    /// @brief allows to forbid the call to RevertChanges() and CommitChanges() if Execute() has not been succesfully
    /// called on that instance
    bool revertAndCommitLocker_ = true;
};

}  // namespace pkgmgr
}  // namespace ucm
}  // namespace ara

#endif  // ARA_UCM_PKGMGR_STORAGE_SWCL_BASE_ACTION_H_
