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

#ifndef ARA_UCM_PKGMGR_STORAGE_SWCLMANAGER_H_
#define ARA_UCM_PKGMGR_STORAGE_SWCLMANAGER_H_

#include "ara/core/vector.h"
#include "parsing/software_cluster.h"

namespace ara
{
namespace ucm
{
namespace pkgmgr
{

using ara::core::Result;

/// @brief Manager of software packages in the system
///
/// @uptrace{SWS_UCM_00122}
class SWCLManager
{
public:
    virtual ~SWCLManager() = default;

    /// @brief Return all software packages in state kPresent
    ///
    /// @returns vector of packages
    virtual ara::core::Vector<SoftwareCluster> GetPresentSWCLs() const = 0;

    /// @brief Return all processes that exist in SWCLs in state kPresent, kAdded and kUpdated.
    ///
    /// @returns vector of applications (i.e. processes)
    virtual ara::core::Vector<SoftwareCluster> GetSWCLsForActivation() const = 0;

    /// @brief Gets the software clusters change info vector
    ///
    /// @returns vector of software clusters info
    virtual const SwClusterInfoVectorType& GetSWCLsChangeInfo() const = 0;

    /// @brief Adds new software cluster change info
    ///
    /// @returns nothing
    virtual void AddSWCLChangeInfo(SoftwarePackage const& softwarePackage) = 0;

    /// @brief Resets the software clusters change info. vector
    ///
    /// @returns nothing
    virtual void ResetSWCLChangeInfo() = 0;
};

}  // namespace pkgmgr
}  // namespace ucm
}  // namespace ara

#endif  // ARA_UCM_PKGMGR_STORAGE_SWCLMANAGER_H_
