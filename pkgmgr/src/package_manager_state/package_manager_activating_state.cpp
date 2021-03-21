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

#include "package_manager_state/package_manager_verifying_state.h"
#include "package_manager_state/package_manager_activating_state.h"
#include "package_manager_state/package_manager_ready_state.h"
#include "package_manager/package_manager.h"
#include "package_manager_impl/package_manager_impl.h"

namespace ara
{
namespace ucm
{
namespace pkgmgr
{

PackageManagerActivatingState::PackageManagerActivatingState()
{
    SaveCurrentState(this->GetStatus());
    log_.LogInfo() << "Package Manager Activating State";
}

void PackageManagerActivatingState::OnSuccess(PackageManager& pm, StateAccessor& accessor)
{
    accessor.Reset(std::make_unique<PackageManagerVerifyingState>());
    pm.StartVerification();
}

void PackageManagerActivatingState::OnFailure(StateAccessor& accessor)
{
    // should be READY when it is implemented
    accessor.Reset(std::make_unique<PackageManagerReadyState>());
}

}  // namespace pkgmgr
}  // namespace ucm
}  // namespace ara
