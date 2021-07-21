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

#ifndef ARA_UCM_PKGMGR_PARSING_SOFTWARE_CLUSTER_H_
#define ARA_UCM_PKGMGR_PARSING_SOFTWARE_CLUSTER_H_

#include "ara/core/string.h"
#include "ara/core/vector.h"

#include "parsing/version.h"
#include "parsing/application.h"
#include "parsing/parsing_types.h"
#include "parsing/software_cluster_manifest.h"

namespace ara
{
namespace ucm
{
namespace pkgmgr
{
using String = ara::core::String;
/// @brief This class represents an adaptive platform Software Cluster
class SoftwareCluster
{
public:
    /// @brief Initializes a new Software Cluster.
    ///
    /// @param swclManifest The Manifest of this SoftwareCluster
    /// @param manifestPath The absolte path to the manifest file
    SoftwareCluster(const SoftwareClusterManifest& swclManifest, const String& manifestPath);

    virtual ~SoftwareCluster() = default;

    /// @brief Install a previously not installed application to the platform
    virtual void InstallApplication() const;

    /// @brief Remove an application from the platform
    virtual void RemoveApplication() const;

    /// @brief Update an application already present on the platform
    virtual void UpdateApplication() const;

    /// @brief Returns the dependencies of this application.
    ///
    /// The dependencies are defined in the application manifest
    ///
    /// @returns List of swcl
    virtual const ara::core::Vector<SoftwareCluster>& GetDependencies() const;

    /// @brief Returns the manifest of this Software Cluster
    ///
    /// @returns the Software Cluster Manifest
    virtual const SoftwareClusterManifest& GetSwclManifest() const;

    /// @brief Returns the state of this Software Cluster.
    ///
    /// @returns Software Cluster state
    virtual const SwClusterStateType& GetSwclState() const noexcept;

    /// @brief Returns the absolute path to this Software Cluster's manifest file.
    ///
    /// @returns absolute path to manifest
    const String& GetManifestPath() const noexcept;

    /// @brief Set the state of Software Cluster
    virtual void SetSwclState(SwClusterStateType swclState);

protected:
    /// @brief Parses the software cluster manifest file.
    ///
    /// Try to parse the Software Cluster's manifest file and initialize the list of dependencies.
    /// The path to this file has been specified in the constructor.
    virtual void ParseSwclManifest();

    /// @brief List of dependencies for this Software Cluster.
    ///
    /// This list is filled when parsing the manifest file
    ara::core::Vector<SoftwareCluster> dependencies_;

    /// @brief List of applications inside this Software Cluster
    ara::core::Vector<Application> applications_;

    /// @brief The state of Software Cluster
    SwClusterStateType swclState_;

    /// @brief Remove the complete Software Cluster from the platform.
    ///
    /// Called internally when all applications of this software cluster have been removed
    virtual void SelfDestruct() const;

private:
    /// @brief parsed Software Cluster manifest of this Software Cluster
    const SoftwareClusterManifest swclManifest_;

    /// @brief Absolute path to this Software Cluster's manifest file
    String manifestPath_;
};

}  // namespace pkgmgr
}  // namespace ucm
}  // namespace ara

#endif  // ARA_UCM_PKGMGR_PARSING_SOFTWARE_CLUSTER_H_
