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

#include "storage/install_action.h"
#include "parsing/software_package.h"
#include "filesystem.h"

namespace ara
{
namespace ucm
{
namespace pkgmgr
{
using String = ara::core::String;

Result<void, ErrorCode> InstallAction::Execute()
{  // creates the parent folder of the installed swcl e.g. /usr/var/apdtest/ucm/optupdateDir/swcls/swcl_EBA
    const String finalDestPathParentDirectory
        = finalDestinationPath_.substr(0, finalDestinationPath_.find_last_of("/"));
    bool intermediateRes = boost::filesystem::create_directories(finalDestPathParentDirectory.c_str());

    if (!intermediateRes) {
        // verifies that the parent folder of the intalled swcl has correctly been created
        return Result<void>::FromError(UCMErrorDomainErrc::kProcessedSoftwarePackageInconsistent);
    } else {  // creates the final directory in which the installed swcl will be stored e.g.
              // ucm/opt/updateDir/swcls/swcl_EBA/1.5.0
        // and moves inside this directory the content of the folder containing the extracted software package
        auto res = fs_.MoveDirectory(package_.GetExtractionPath(), finalDestinationPath_);

        if (!res.HasValue()) {
            // verifies that the content of the folder containing the extracted software package has correctly been
            // copied recursively into the final directory
            return Result<void>::FromError(UCMErrorDomainErrc::kProcessedSoftwarePackageInconsistent);
        } else {
            revertAndCommitLocker_ = false;
            return {};
        }
    }
}

Result<void, ErrorCode> InstallAction::RevertChanges()
{
    // security to avoid a RevertChanges() Call in case the Execute() has failed
    if (revertAndCommitLocker_ == false) {
        // remove Directory of the newly installed SWCL and check for errors
        Result<void> removeDirError = fs_.RemoveDirectory(finalDestinationPath_);
        bool checkRemoveDirError = (removeDirError.CheckError(FilesystemErrc::kNoSuchFileOrDirectory)
            || removeDirError.CheckError(FilesystemErrc::kRemoveDirectory));
        if (!checkRemoveDirError) {
            revertAndCommitLocker_ = true;
            return {};
        } else {
            return Result<void>::FromError(UCMErrorDomainErrc::kProcessedSoftwarePackageInconsistent);
        }
    } else {
        log_.LogWarn() << "cannot revert after a previous unsuccesful Execute()";
        return Result<void>::FromError(UCMErrorDomainErrc::kProcessedSoftwarePackageInconsistent);
    }
}

Result<void, ErrorCode> InstallAction::CommitChanges()
{
    // noop since the SWCL is kept
    return {};
}

}  // namespace pkgmgr
}  // namespace ucm
}  // namespace ara
