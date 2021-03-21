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

#ifndef ARA_UCM_PKGMGR_MANIFEST_SAMPLES_H_
#define ARA_UCM_PKGMGR_MANIFEST_SAMPLES_H_

#include "ara/core/string.h"
#include "boost/property_tree/ptree.hpp"
#include "parsing/software_package_manifest.h"
#include "parsing/software_cluster_manifest.h"
#include "parsing/software_cluster_manifest_parser.h"
#include "parsing/software_package_manifest_parser.h"

namespace ara
{
namespace ucm
{
namespace pkgmgr
{
namespace testdata
{

using String = ara::core::String;
using boost::property_tree::ptree;

extern const String sample_application_manifest;
extern const String sample_application_manifest_not_met;
extern const String sample_application_manifest_no_dep;
extern const String sample_swpackage_manifest;
extern const String sample_processes_list;

ptree CreateSoftwarePackageManifest(const String& action);
SoftwarePackageManifest CreateSoftwarePackageManifestObject(const String& action);
String ElementsAsCommaSeparatedString(ara::core::Vector<String> elements);
ptree CreateSoftwareClusterManifest(const String& swclName, const String& version = String("1.0"));
SoftwareClusterManifest CreateSoftwareClusterManifestObject(const String& swclName,
    const String& version = String("1.0"));

}  // namespace testdata
}  // namespace pkgmgr
}  // namespace ucm
}  // namespace ara

#endif  // ARA_UCM_PKGMGR_MANIFEST_SAMPLES_H_
