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

#include "parsing/software_cluster_manifest.h"

#include "parsing/version.h"

namespace ara
{
namespace ucm
{
namespace pkgmgr
{

SoftwareClusterManifest::SoftwareClusterManifest(const String& category,
    const String& changes,
    const String& license,
    const Version& previousVersion,
    const String& shortName,
    const String& uuid,
    const String& vendorID,
    const Version& version,
    const ara::core::Vector<String>& dependentSoftwareClusters)
    : category_(category)
    , changes_(changes)
    , license_(license)
    , previousVersion_(previousVersion)
    , shortName_(shortName)
    , uuid_(uuid)
    , vendorID_(vendorID)
    , version_(version)
    , dependentSoftwareClusters_(dependentSoftwareClusters)
{}

const ara::core::Vector<String> SoftwareClusterManifest::GetSoftwareClusterDependencies() const noexcept
{
    return dependentSoftwareClusters_;
}
String SoftwareClusterManifest::GetCategory() const
{
    return category_;
}
String SoftwareClusterManifest::GetChanges() const
{
    return changes_;
}
String SoftwareClusterManifest::GetLicense() const
{
    return license_;
}
Version SoftwareClusterManifest::GetPreviousVersion() const
{
    return previousVersion_;
}
String SoftwareClusterManifest::GetShortName() const
{
    return shortName_;
}
String SoftwareClusterManifest::GetUuid() const
{
    return uuid_;
}
String SoftwareClusterManifest::GetVendorID() const
{
    return vendorID_;
}
Version SoftwareClusterManifest::GetVersion() const
{
    return version_;
}

}  // namespace pkgmgr
}  // namespace ucm
}  // namespace ara
