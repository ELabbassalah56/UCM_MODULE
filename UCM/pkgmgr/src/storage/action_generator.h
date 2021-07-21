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

#ifndef ARA_UCM_PKGMGR_STORAGE_ACTION_GENERATOR_H_
#define ARA_UCM_PKGMGR_STORAGE_ACTION_GENERATOR_H_

#include <memory>
#include "storage/reversible_action.h"
#include "parsing/software_package_manifest.h"
#include "ara/ucm/pkgmgr/impl_type_actiontype.h"

namespace ara
{
namespace ucm
{
namespace pkgmgr
{

/// @brief Generator for creating storage actions.
///
/// Storage actions for regular SWCL use different directories
/// in a filesystem to organize the extraction of the existing
/// platform before the changes are applied to the system.
/// Storage actions for system updates will be implemented by
/// special actions.
///
/// @uptrace{SWS_UCM_00001}
class ActionGenerator
{
public:
    /// @brief Create a processing action for a software package
    /// @param destinationRoot Abolute path to the directory where to install all the SWCLs to.
    /// @param p The input package for the action
    ///
    /// Builds the appropriate reversible action based on software package data
    ///
    /// @return An action implementing functions Execute and Cleanup
    std::unique_ptr<ReversibleAction> operator()(const ara::core::String& destinationRoot,
        const SoftwarePackage& p) const;
};

}  // namespace pkgmgr
}  // namespace ucm
}  // namespace ara

#endif  // ARA_UCM_PKGMGR_STORAGE_ACTION_GENERATOR_H_
