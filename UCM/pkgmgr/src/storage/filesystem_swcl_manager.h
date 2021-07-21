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

#ifndef ARA_UCM_PKGMGR_STORAGE_FILESYSTEMSWCLMANAGER_H_
#define ARA_UCM_PKGMGR_STORAGE_FILESYSTEMSWCLMANAGER_H_

#include "ara/core/vector.h"
#include "storage/reversible_action.h"
#include "swcl_manager.h"

namespace ara
{
namespace ucm
{
namespace pkgmgr
{

/// @brief software cluster manager working with the filesystem directly
class FileSystemSWCLManager : public SWCLManager
{
public:
    explicit FileSystemSWCLManager(ara::core::String rootDirectory);

    /// <inheritdoc>
    virtual ara::core::Vector<SoftwareCluster> GetPresentSWCLs() const override;

    /// <inheritdoc>
    ara::core::Vector<SoftwareCluster> GetSWCLsForActivation() const override;

    /// @brief Add one action to the queue of actions.
    ///
    /// Adds one action to the queue of the actions to be executed. Empty actions (nullptr) are ignored.
    virtual void AddAction(std::unique_ptr<ReversibleAction> action);

    /// @brief Persist all actions to the filesystem
    virtual void PersistActionsToFilesystem();

    /// @brief Recover all actions from the filesystem
    virtual void RecoverActionsFromFilesystem();

    /// @brief Commits all actions
    ///
    /// @returns nothing (see the Result<...> class) in case of success, otherwise see ErrorCode
    virtual Result<void, ErrorCode> CommitChanges();

    /// <inheritdoc>
    virtual const SwClusterInfoVectorType& GetSWCLsChangeInfo() const override;

    /// @brief Reverts all actions
    ///
    /// @returns nothing (see the Result<...> class) in case of success, otherwise see ErrorCode
    virtual Result<void, ErrorCode> RevertChanges();

    /// <inheritdoc>
    virtual void AddSWCLChangeInfo(SoftwarePackage const& softwarePackage) override;

    /// <inheritdoc>
    virtual void ResetSWCLChangeInfo() override;

private:
    /// @brief Storage for all actions
    ara::core::Vector<std::unique_ptr<ReversibleAction>> actions_;

    ara::core::String rootDirectory_;

    /// @brief Storage for software clusters change info
    SwClusterInfoVectorType swclChangeInfo_;

    /// @brief Parse system software packages' data and cache it
    ara::core::Vector<SoftwareCluster> const IndexActionsAndSWCLs() const;
};

}  // namespace pkgmgr
}  // namespace ucm
}  // namespace ara

#endif  // ARA_UCM_PKGMGR_STORAGE_FILESYSTEMSWCLMANAGER_H_
