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

#ifndef ARA_UCM_PKGMGR_PARSING_SOFTWARE_PACKAGE_MANIFEST_H_
#define ARA_UCM_PKGMGR_PARSING_SOFTWARE_PACKAGE_MANIFEST_H_

#include "ara/core/string.h"
#include "ara/ucm/pkgmgr/impl_type_actiontype.h"
#include "parsing/version.h"

namespace ara
{
namespace ucm
{
namespace pkgmgr
{

/// @brief Manifest of a software package
///
/// @uptrace{SWS_UCM_00029}
class SoftwarePackageManifest
{
public:
    SoftwarePackageManifest(  // all fields are required
        const ActionType& actionType,
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
        const ara::core::String& uuid);

    const ActionType& GetActionType() const;
    const ara::core::String& GetActivationAction() const;
    const ara::core::String& GetCategory() const;
    const ara::core::String& GetCompressedSoftwarePackageSize() const;
    const ara::core::String& GetDiagnosticAddress() const;
    const ara::core::String& GetIsDeltaPackage() const;
    const Version& GetMinUCMSupportedVersion() const;
    const Version& GetMaxUCMSupportedVersion() const;
    const ara::core::String& GetPackagerID() const;
    const ara::core::String& GetShortName() const;
    const ara::core::String& GetTypeApproval() const;
    const ara::core::String& GetUcmIdentifier() const;
    const ara::core::String& GetUncompressedSoftwarePackageSize() const;
    const ara::core::String& GetUuid() const;

private:
    ActionType actionType_;
    ara::core::String activationAction_;
    ara::core::String category_;
    ara::core::String compressedSoftwarePackageSize_;
    ara::core::String diagnosticAddress_;
    ara::core::String isDeltaPackage_;
    Version minUCMSupportedVersion_;
    Version maxUCMSupportedVersion_;
    ara::core::String packagerID_;
    ara::core::String shortName_;
    ara::core::String typeApproval_;
    ara::core::String ucmIdentifier_;
    ara::core::String uncompressedSoftwarePackageSize_;
    ara::core::String uuid_;
};

}  // namespace pkgmgr
}  // namespace ucm
}  // namespace ara

#endif  // ARA_UCM_PKGMGR_PARSING_SOFTWARE_PACKAGE_MANIFEST_H_
