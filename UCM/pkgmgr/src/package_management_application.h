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

#ifndef ARA_UCM_PKGMGR_PACKAGE_MANAGEMENT_APPLICATION_H_
#define ARA_UCM_PKGMGR_PACKAGE_MANAGEMENT_APPLICATION_H_

#include <memory>
#include "adaptive_application.h"
#include "ara/log/logging.h"
#include "ara/ucm/pkgmgr/packagemanagement_skeleton.h"
#include "ara/core/string.h"

namespace ara
{
namespace ucm
{
namespace pkgmgr
{

/// @brief This class represents
/// Package Management Application
// class PackageManagementApplication final : public AdaptiveApplication
class PackageManagementApplication : public AdaptiveApplication
{

public:
    PackageManagementApplication();

    PackageManagementApplication(const PackageManagementApplication&) = delete;
    PackageManagementApplication& operator=(const PackageManagementApplication&) = delete;

    PackageManagementApplication(PackageManagementApplication&&) = delete;
    PackageManagementApplication& operator=(PackageManagementApplication&&) = delete;

    ~PackageManagementApplication() = default;

    /// @brief The location of the Service Instance Manifest File to parse
    const ara::core::String serviceDeploymentManifestPath_ = "/opt/package_manager/etc/vsomeip.json";

    /// @brief Parses the given Manifest file and returns the value of the instance ID
    uint16_t GetServiceInstanceId(const ara::core::String& ManifestPath);

protected:
    bool OnInitialize() override;
    void Run() override;
    void OnTerminate() override;

private:
    /// @brief Pointer to Package Management service skeleton
    std::unique_ptr<ara::ucm::pkgmgr::skeleton::PackageManagementSkeleton> service_;

    /// @brief A reference to logger, which is used to log app lifetime info
    ara::log::Logger& log_;
   
};

}  // namespace pkgmgr
}  // namespace ucm
}  // namespace ara

#endif  // ARA_UCM_PKGMGR_PACKAGE_MANAGEMENT_APPLICATION_H_