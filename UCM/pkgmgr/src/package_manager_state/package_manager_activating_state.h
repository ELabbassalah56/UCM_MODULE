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

#ifndef ARA_UCM_PKGMGR_PACKAGE_MANAGER_STATE_PACKAGE_MANAGER_ACTIVATING_STATE_H_
#define ARA_UCM_PKGMGR_PACKAGE_MANAGER_STATE_PACKAGE_MANAGER_ACTIVATING_STATE_H_

#include <memory>

#include "package_manager_state/package_manager_state.h"
#include "transfer/receive_types.h"
//#include "package_manager_impl/package_manager_impl.h"

namespace ara
{
namespace ucm
{
namespace pkgmgr
{

/// @brief Class representing Activating state of Package Manager
///
/// @uptrace{SWS_UCM_00084}
/// @uptrace{SWS_UCM_00153}
class PackageManagerActivatingState final : public PackageManagerState
{
public:
    /// @brief Creates a new instance of Activating state and saves the current state
    PackageManagerActivatingState();
    ~PackageManagerActivatingState() = default;
    /// <inheritdoc>
    void OnSuccess(PackageManager& pm, StateAccessor& accessor) override;

    /// <inheritdoc>
    void OnFailure(StateAccessor& accessor) override;

    /// <inheritdoc>
    PackageManagerStatusType GetStatus() const override
    {
        return PackageManagerStatusType::kActivating;
    }
};

}  // namespace pkgmgr
}  // namespace ucm
}  // namespace ara

#endif  // ARA_UCM_PKGMGR_PACKAGE_MANAGER_STATE_PACKAGE_MANAGER_ACTIVATING_STATE_H_
