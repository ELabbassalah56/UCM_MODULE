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

#include "parsing/software_package_parser_impl.h"

#include "ara/core/string.h"
#include "ara/core/vector.h"
#include <memory>
#include "parsing/software_package.h"
#include "parsing/software_package_manifest.h"
#include "parsing/application_list_builder.h"
#include "parsing/software_cluster_manifest.h"
#include "parsing/software_cluster_manifest_parser.h"
#include "parsing/software_package_manifest_parser.h"

namespace ara
{
namespace ucm
{
namespace pkgmgr
{

SoftwarePackageParserImpl::SoftwarePackageParserImpl(Filesystem& fs,
    SoftwareClusterManifestParser& swclManifestParser,
    SoftwarePackageManifestParser& swPackageManifestParser)
    : fs_(fs)
    , swclManifestParser_(swclManifestParser)
    , swPackageManifestParser_(swPackageManifestParser)
{}

std::unique_ptr<SoftwarePackage> SoftwarePackageParserImpl::Parse(const String& extractionDirectory)
{
    String swpkgPath = extractionDirectory;
    String swclPath = swpkgPath;

    auto manifest = swPackageManifestParser_.ParseFromFile(swpkgPath + "/SWPKG_MANIFEST.json");
    auto swclManifest = swclManifestParser_.ParseFromFile(swclPath + "/SWCL_MANIFEST.json");

    ara::core::String swclManifestPath = extractionDirectory + "/SWCL_MANIFEST.json";

    SoftwareCluster swCluster(*swclManifest, swclManifestPath);

    ApplicationListBuilder builder(fs_, swclPath);
    ara::core::Vector<Application> apps = builder.GetApplications();

    return std::make_unique<SoftwarePackage>(*manifest, extractionDirectory, swCluster);
}

}  // namespace pkgmgr
}  // namespace ucm
}  // namespace ara
