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

#ifndef ARA_UCM_PKGMGR_PARSING_SOFTWARE_CLUSTER_MANIFEST_H_
#define ARA_UCM_PKGMGR_PARSING_SOFTWARE_CLUSTER_MANIFEST_H_

#include "ara/core/string.h"
#include "ara/core/vector.h"
#include "parsing/version.h"

namespace ara
{
namespace ucm
{
namespace pkgmgr
{
using String = ara::core::String;
/// @brief Data structure representing the manifest of a software cluster.
///
/// @uptrace{SWS_UCM_00112}
class SoftwareClusterManifest
{
public:
    /// @brief Creates a new software cluster manifest.
    ///
    /// @param name The name of this software cluster
    /// @param version The version of this software cluster
    /// @param checksum The checksum of this software cluster
    /// @param uncompressedSize The size of this software cluster in bytes (uncompressed)
    /// @param referencedApplications The applications part of this software cluster
    /// @param dependentSoftwareClusters The names of dependent software clusters
    SoftwareClusterManifest(const String& category,
        const String& changes,
        const String& license,
        const Version& previousVersion,
        const String& shortName,
        const String& uuid,
        const String& vendorID,
        const Version& version,
        const ara::core::Vector<String>& dependentSoftwareClusters);

    /// @brief Return the names of dependent software clusters.
    ///
    /// @returns Names of dependent software clusters
    const ara::core::Vector<String> GetSoftwareClusterDependencies() const noexcept;

    String GetCategory() const;
    String GetChanges() const;
    String GetLicense() const;
    Version GetPreviousVersion() const;
    String GetShortName() const;
    String GetUuid() const;
    String GetVendorID() const;
    Version GetVersion() const;

private:
    String category_;
    String changes_;
    String license_;
    Version previousVersion_;
    String shortName_;
    String uuid_;
    String vendorID_;
    Version version_;
    /// @brief The names of software clusters that this cluster depends on.
    const ara::core::Vector<String> dependentSoftwareClusters_;
};

}  // namespace pkgmgr
}  // namespace ucm
}  // namespace ara

#endif  // ARA_UCM_PKGMGR_PARSING_SOFTWARE_CLUSTER_MANIFEST_H_
