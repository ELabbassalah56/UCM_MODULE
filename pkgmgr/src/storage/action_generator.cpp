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

#include "action_generator.h"
#include "install_action.h"
#include "remove_action.h"
#include "update_action.h"

namespace ara
{
namespace ucm
{
namespace pkgmgr
{

std::unique_ptr<ReversibleAction> ActionGenerator::operator()(const ara::core::String& destinationPath,
    const SoftwarePackage& softwarePackage) const
{
    /// collect the information needed to create the proper path
    ara::core::String swcl_name = softwarePackage.GetSoftwareCluster().GetSwclManifest().GetShortName();
    Version swcl_version = softwarePackage.GetSoftwareCluster().GetSwclManifest().GetVersion();

    ActionType action_type = softwarePackage.GetManifest().GetActionType();

    ara::core::String targetPathFinal = destinationPath + "/" + swcl_name + "/" + swcl_version.ToString();

    switch (action_type) {
    case ActionType::kInstall:
        return std::make_unique<InstallAction>(targetPathFinal, softwarePackage);
        break;
    case ActionType::kRemove:
        return std::make_unique<RemoveAction>(targetPathFinal, softwarePackage);
        break;
    case ActionType::kUpdate:
        return std::make_unique<UpdateAction>(targetPathFinal, softwarePackage);
        break;
    default:
        return nullptr;
        break;
    }
}

}  // namespace pkgmgr
}  // namespace ucm
}  // namespace ara
