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

#include "parsing/software_package_manifest.h"

namespace ara
{
namespace ucm
{
namespace pkgmgr
{

SoftwarePackageManifest::SoftwarePackageManifest(const ActionType& actionType,
    const ara::core::String& activationAction,
    const ara::core::String& category,
    const ara::core::String& compressedSoftwarePackageSize,
    const ara::core::String& diagnosticAddress,
    const ara::core::String& isDeltaPackage,
    const Version& minUCMSupportedVersion,
    const Version& maxUCMSupportedVersion,
    const ara::core::String& packagerID,
    const ara::core::String& shortName,
    const ara::core::String& typeApproval,
    const ara::core::String& ucmIdentifier,
    const ara::core::String& uncompressedSoftwarePackageSize,
    const ara::core::String& uuid)
    : actionType_(actionType)
    , activationAction_(activationAction)
    , category_(category)
    , compressedSoftwarePackageSize_(compressedSoftwarePackageSize)
    , diagnosticAddress_(diagnosticAddress)
    , isDeltaPackage_(isDeltaPackage)
    , minUCMSupportedVersion_(minUCMSupportedVersion)
    , maxUCMSupportedVersion_(maxUCMSupportedVersion)
    , packagerID_(packagerID)
    , shortName_(shortName)
    , typeApproval_(typeApproval)
    , ucmIdentifier_(ucmIdentifier)
    , uncompressedSoftwarePackageSize_(uncompressedSoftwarePackageSize)
    , uuid_(uuid)
{}

const ActionType& SoftwarePackageManifest::GetActionType() const
{
    return actionType_;
}

const ara::core::String& SoftwarePackageManifest::GetActivationAction() const
{
    return activationAction_;
}

const ara::core::String& SoftwarePackageManifest::GetCategory() const
{
    return category_;
}

const ara::core::String& SoftwarePackageManifest::GetCompressedSoftwarePackageSize() const
{
    return compressedSoftwarePackageSize_;
}

const ara::core::String& SoftwarePackageManifest::GetDiagnosticAddress() const
{
    return diagnosticAddress_;
}

const ara::core::String& SoftwarePackageManifest::GetIsDeltaPackage() const
{
    return isDeltaPackage_;
}

const Version& SoftwarePackageManifest::GetMinUCMSupportedVersion() const
{
    return minUCMSupportedVersion_;
}

const Version& SoftwarePackageManifest::GetMaxUCMSupportedVersion() const
{
    return maxUCMSupportedVersion_;
}

const ara::core::String& SoftwarePackageManifest::GetPackagerID() const
{
    return packagerID_;
}

const ara::core::String& SoftwarePackageManifest::GetShortName() const
{
    return shortName_;
}

const ara::core::String& SoftwarePackageManifest::GetTypeApproval() const
{
    return typeApproval_;
}

const ara::core::String& SoftwarePackageManifest::GetUcmIdentifier() const
{
    return ucmIdentifier_;
}

const ara::core::String& SoftwarePackageManifest::GetUncompressedSoftwarePackageSize() const
{
    return uncompressedSoftwarePackageSize_;
}

const ara::core::String& SoftwarePackageManifest::GetUuid() const
{
    return uuid_;
}

}  // namespace pkgmgr
}  // namespace ucm
}  // namespace ara
