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

#include "storage/update_action.h"
#include "parsing/software_package.h"
#include "filesystem.h"

namespace ara
{
namespace ucm
{
namespace pkgmgr
{
using String = ara::core::String;

Result<void, ErrorCode> UpdateAction::Execute()
{  // get the parent folder of the installed swcl e.g. /usr/var/apdtest/ucm/opt/updateDir/swcls/swcl_EBA
    const String finalDestPathParentDirectory
        = finalDestinationPath_.substr(0, finalDestinationPath_.find_last_of("/"));

    // verifies that the swcl is already existing on the target, otherwise, abort the update action
    if (!fs_.DoesDirectoryExist(finalDestPathParentDirectory)) {
        log_.LogWarn()
            << "swcl not previously installed on the target, need to invoke InstallAction instead of UpdateAction";
        return Result<void>::FromError(UCMErrorDomainErrc::kProcessedSoftwarePackageInconsistent);

    } else {
        // verifies that the same version of this swcl has not been previously installed
        if (fs_.DoesDirectoryExist(finalDestinationPath_)) {
            log_.LogWarn() << "same version of the swcl is already installed";
            return Result<void>::FromError(UCMErrorDomainErrc::kProcessedSoftwarePackageInconsistent);

        } else {  // creates the final directory in which the updated swcl will be stored e.g.
                  // ucm/opt/updateDir/swcls/swcl_EBA/2.0.0
            // and moves inside this directory the content of the folder containing the extracted software package
            auto res = fs_.MoveDirectory(package_.GetExtractionPath(), finalDestinationPath_);

            if (res.HasValue()) {
                revertAndCommitLocker_ = false;
                return {};
            } else {
                // verifies that the content of the folder containing the extracted software package has correctly been
                // copied recursively into the final directory
                return Result<void>::FromError(UCMErrorDomainErrc::kProcessedSoftwarePackageInconsistent);
            }
        }
    }
}

Result<void, ErrorCode> UpdateAction::RevertChanges()
{
    // security to avoid a RevertChanges() Call in case the Execute() has failed or the CommitChange() has succeeded
    if (!revertAndCommitLocker_) {
        // removes the newly installed SWCL version
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

Result<void, ErrorCode> UpdateAction::CommitChanges()
{
    // get the parent folder of the installed swcl e.g. /usr/var/apdtest/ucm/opt/updateDir/swcls/swcl_EBA
    const String finalDestPathParentDirectory
        = finalDestinationPath_.substr(0, finalDestinationPath_.find_last_of("/"));

    // security to avoid a CommitChanges() Call in case the Execute() has failed or the RevertChange() has succeeded
    if (!revertAndCommitLocker_) {
        // make sure the new directory (i.e. the new version of the swcl has not been accidentaly removed in the
        // meantime
        if (fs_.DoesDirectoryExist(finalDestinationPath_)) {
            // remove Directory of the old version(s) of the SWCL and check for errors

            // list all versions of the swcl that are currently installed (usually only 2 versions shall be there, the
            // old and the new one)
            ara::core::Vector<ara::core::String> swclVersionList = fs_.GetSubdirectories(finalDestPathParentDirectory);

            // scan this list and remove all directories of the versions that are not the most recent one (i.e. the one
            // installed by the previous Execute())
            for (const auto& swclVersion : swclVersionList) {
                if (swclVersion != finalDestinationPath_) {
                    Result<void> removeDirError = fs_.RemoveDirectory(swclVersion);
                    bool checkRemoveDirError = (removeDirError.CheckError(FilesystemErrc::kNoSuchFileOrDirectory)
                        || removeDirError.CheckError(FilesystemErrc::kRemoveDirectory));
                    if (!checkRemoveDirError) {
                        // nothing to do except to iterate over the swclVersionList;
                    } else {
                        return Result<void>::FromError(UCMErrorDomainErrc::kProcessedSoftwarePackageInconsistent);
                    }
                }
            }
            revertAndCommitLocker_ = true;
            return {};
        } else {
            log_.LogWarn() << "cannot remove old version as the new version does not exist";
            return Result<void>::FromError(UCMErrorDomainErrc::kProcessedSoftwarePackageInconsistent);
        }

    } else {
        log_.LogWarn() << "cannot commit after a previous unsuccesful Execute() or a succesful CommitCHanges()";
        return Result<void>::FromError(UCMErrorDomainErrc::kProcessedSoftwarePackageInconsistent);
    }
}

}  // namespace pkgmgr
}  // namespace ucm
}  // namespace ara
