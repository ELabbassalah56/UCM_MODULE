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

#ifndef ARA_UCM_PKGMGR_PARSING_SOFTWARE_PACKAGE_H_
#define ARA_UCM_PKGMGR_PARSING_SOFTWARE_PACKAGE_H_

#include "ara/core/string.h"
#include "ara/core/vector.h"
#include <algorithm>

#include "parsing/software_package_manifest.h"
#include "parsing/application.h"
#include "parsing/software_cluster.h"

namespace ara
{
namespace ucm
{
namespace pkgmgr
{
using String = ara::core::String;
/// @brief A software package is the unit of deployment for Updates.
class SoftwarePackage
{
public:
    /// @brief Initialize a SoftwarePackage.
    ///
    /// @param manifest The parsed SoftwarePackage manifest
    /// @param path The full file path to the extracted software package archive
    /// @param softwareCluster The software cluster contained in this software package
    ///
    /// @uptrace{SWS_UCM_00122}
    SoftwarePackage(const SoftwarePackageManifest& manifest,
        const String& path,
        const SoftwareCluster& softwareCluster);

    virtual ~SoftwarePackage() = default;

    /// @brief Return the absolute path to the extracted software package archive.
    ///
    /// @returns Absolute path to the location where this software package archive has been extracted to
    virtual const String& GetExtractionPath() const noexcept;

    /// @brief Return the manifest that is part of this software package.
    ///
    /// @returns The manifest of this software package
    virtual const SoftwarePackageManifest& GetManifest() const;

    /// @returns The software cluster of this software package
    virtual const SoftwareCluster& GetSoftwareCluster() const;

private:
    /// @brief The manifest of this software package.
    const SoftwarePackageManifest manifest_;

    /// @brief The abolsute path to the extracted software package.
    const String path_;

    /// @brief The software cluster of this software package.
    const SoftwareCluster softwareCluster_;
};

}  // namespace pkgmgr
}  // namespace ucm
}  // namespace ara

#endif  // ARA_UCM_PKGMGR_PARSING_SOFTWARE_PACKAGE_H_
