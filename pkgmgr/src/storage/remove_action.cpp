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

#include "storage/remove_action.h"

namespace ara
{
namespace ucm
{
namespace pkgmgr
{

Result<void, ErrorCode> RemoveAction::Execute()
{
    // marks the swcl for removal
    if (fs_.DoesDirectoryExist(finalDestinationPath_)) {
        revertAndCommitLocker_ = false;
        return {};
    } else {
        log_.LogWarn() << "swcl not installed in the mentionned directory";
        return Result<void>::FromError(UCMErrorDomainErrc::kProcessedSoftwarePackageInconsistent);
    }
}

Result<void, ErrorCode> RemoveAction::RevertChanges()
{
    if (revertAndCommitLocker_ == false) {
        // Clean-up : removes the temporary data i.e. the folder containing the extracted software package
        Result<void> cleanUpError = fs_.RemoveDirectory(package_.GetExtractionPath());
        bool checkCleanUpError = (cleanUpError.CheckError(FilesystemErrc::kNoSuchFileOrDirectory)
            || cleanUpError.CheckError(FilesystemErrc::kRemoveDirectory));
        if (!checkCleanUpError) {
            // unmark the swcl for removal
            revertAndCommitLocker_ = true;
            return {};
        } else {
            log_.LogWarn() << "error during Clean-Up, removal of the unpacked software package impossible";
            return Result<void>::FromError(UCMErrorDomainErrc::kNotAbleToRevertPackages);
        }
    } else {
        log_.LogWarn() << "error during Revert, Execute() has not been successfully called beforehand";
        return Result<void>::FromError(UCMErrorDomainErrc::kNotAbleToRevertPackages);
    }
}

Result<void, ErrorCode> RemoveAction::CommitChanges()
{
    // remove the swcl directory from its final location
    // get the parent folder of the installed swcl e.g. /usr/var/apdtest/ucm/opt/updateDir/swcls/swcl_EBA
    const String finalDestPathParentDirectory
        = finalDestinationPath_.substr(0, finalDestinationPath_.find_last_of("/"));

    // test to prevent swcl to be removed when RevertChanges() has been called after Execute()
    if (revertAndCommitLocker_ == false) {
        Result<void> removeDirError = fs_.RemoveDirectory(finalDestPathParentDirectory);
        bool checkRemoveDirError = (removeDirError.CheckError(FilesystemErrc::kNoSuchFileOrDirectory)
            || removeDirError.CheckError(FilesystemErrc::kRemoveDirectory));
        if (!checkRemoveDirError) {
            // Clean-up : removes the temporary data i.e. the folder containing the extracted software package
            Result<void> cleanUpError = fs_.RemoveDirectory(package_.GetExtractionPath());
            bool checkCleanUpError = (cleanUpError.CheckError(FilesystemErrc::kNoSuchFileOrDirectory)
                || cleanUpError.CheckError(FilesystemErrc::kRemoveDirectory));
            if (!checkCleanUpError) {
                revertAndCommitLocker_ = true;
                return {};
            } else {
                log_.LogWarn() << "error during Clean-Up, removal of the unpacked software package impossible";
                return Result<void>::FromError(UCMErrorDomainErrc::kGeneralReject);
            }
        } else {
            log_.LogWarn() << "error during removal of the swcl directory";
            return Result<void>::FromError(UCMErrorDomainErrc::kGeneralReject);
        }
    } else {
        log_.LogWarn() << "nothing to commit, Execute() is needed prior to CommitChanges()";
        return Result<void>::FromError(UCMErrorDomainErrc::kGeneralReject);
    }
}

}  // namespace pkgmgr
}  // namespace ucm
}  // namespace ara
