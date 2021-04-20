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

#include "filesystem_swcl_manager.h"
#include "parsing/software_cluster_list_builder.h"
#include "error_domain_ucmerrordomain.h"

#include <algorithm>

namespace ara
{
namespace ucm
{
namespace pkgmgr
{

FileSystemSWCLManager::FileSystemSWCLManager(ara::core::String rootDirectory)
    : rootDirectory_(rootDirectory)
{}

ara::core::Vector<SoftwareCluster> FileSystemSWCLManager::GetPresentSWCLs() const
{
    ara::core::Vector<SoftwareCluster> out;
    const auto clusters = IndexActionsAndSWCLs();

    std::copy_if(clusters.begin(), clusters.end(), std::back_inserter(out), [](SoftwareCluster const& swcl) {
        return swcl.GetSwclState() == SwClusterStateType::kPresent;
    });

    return out;
}

void FileSystemSWCLManager::AddAction(std::unique_ptr<ReversibleAction> action)
{
    if (action) {  // check the input
        actions_.emplace_back(std::move(action));
    }
}

void FileSystemSWCLManager::PersistActionsToFilesystem()
{}

void FileSystemSWCLManager::RecoverActionsFromFilesystem()
{}

Result<void, ErrorCode> FileSystemSWCLManager::CommitChanges()
{
    for (auto action = actions_.begin(); action != actions_.end(); action++) {
        if (*action) {
            auto res = (*action)->CommitChanges();
            if (!res.HasValue()) {
                return Result<void, ErrorCode>::FromError(res.Error());
            }
            return res;
        }
    }
    return {};
}

Result<void, ErrorCode> FileSystemSWCLManager::RevertChanges()
{
    for (auto action = actions_.begin(); action != actions_.end(); action++) {
        if (*action) {
            auto res = (*action)->RevertChanges();
            if (!res.HasValue()) {
                return Result<void, ErrorCode>::FromError(res.Error());
            }
            return res;
        }
    }
    return {};
}

ara::core::Vector<SoftwareCluster> const FileSystemSWCLManager::IndexActionsAndSWCLs() const
{
    Filesystem fs;
    SoftwareClusterListBuilder builder(fs, rootDirectory_);

    return builder.GetSwcls();
}

ara::core::Vector<SoftwareCluster> FileSystemSWCLManager::GetSWCLsForActivation() const
{
    ara::core::Vector<SoftwareCluster> activeClusters;
    const auto currentClusters = IndexActionsAndSWCLs();

    std::copy_if(currentClusters.begin(),
        currentClusters.end(),
        std::back_inserter(activeClusters),
        [](SoftwareCluster const& swcl) {
            if ((swcl.GetSwclState() == SwClusterStateType::kPresent)
                || (swcl.GetSwclState() == SwClusterStateType::kAdded)
                || (swcl.GetSwclState() == SwClusterStateType::kUpdated)
               ) {
                return true;
            } else {
                return false;
            }
        });

    return activeClusters;
}

const SwClusterInfoVectorType& FileSystemSWCLManager::GetSWCLsChangeInfo() const
{
    return swclChangeInfo_;
}

void FileSystemSWCLManager::AddSWCLChangeInfo(SoftwarePackage const& softwarePackage)
{
    SwClusterInfoType swclInfo;

    swclInfo.Name = softwarePackage.GetSoftwareCluster().GetSwclManifest().GetShortName();
    swclInfo.Version = softwarePackage.GetSoftwareCluster().GetSwclManifest().GetVersion().ToString();

    // Once SWCL state is completely implemented, GetSwclState() should be used instead.
    switch (softwarePackage.GetManifest().GetActionType()) {
    case ActionType::kUpdate:
        swclInfo.State = SwClusterStateType::kUpdated;
        break;
    case ActionType::kInstall:
        swclInfo.State = SwClusterStateType::kAdded;
        break;
    case ActionType::kRemove:
        swclInfo.State = SwClusterStateType::kRemoved;
        break;
    }

    swclChangeInfo_.push_back(std::move(swclInfo));
}

void FileSystemSWCLManager::ResetSWCLChangeInfo()
{
    swclChangeInfo_.clear();
}

}  // namespace pkgmgr
}  // namespace ucm
}  // namespace ara
