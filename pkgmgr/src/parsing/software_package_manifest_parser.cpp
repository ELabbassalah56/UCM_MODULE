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

#include "parsing/software_package_manifest_parser.h"

#include "boost/property_tree/ptree.hpp"
#include "boost/property_tree/detail/file_parser_error.hpp"
#include "ara/core/string.h"
#include <memory>
#include "ara/log/logging.h"
#include "exceptions.h"
#include "helper.h"

using boost::property_tree::ptree;
using boost::property_tree::ptree_error;
using boost::property_tree::file_parser_error;

namespace ara
{
namespace ucm
{
namespace pkgmgr
{
using String = ara::core::String;
static ara::log::Logger& logger()
{
    static ara::log::Logger& loggerInstance = ara::log::CreateLogger("SPMP", "SoftwarePackageManifestParser context");
    return loggerInstance;
}

std::unique_ptr<SoftwarePackageManifest> SoftwarePackageManifestParser::Parse(
    const boost::property_tree::ptree& manifest)
{
    try {
        return ParseFromPtree(manifest);
    } catch (const ptree_error& parseError) {
        String message = helper::CreateErrorMessage("Error parsing software cluster manifest", parseError.what());
        logger().LogError() << message;
        throw exception::InvalidManifestException(message);
    }
}

std::unique_ptr<SoftwarePackageManifest> SoftwarePackageManifestParser::ParseFromPtree(
    const boost::property_tree::ptree& manifestTree)
{
    // The function is basically a stub. Later it will become more complex with real types.
    return std::make_unique<SoftwarePackageManifest>(  // all fields are required
        ParseActionType(manifestTree),
        manifestTree.get<String>("activationAction"),
        manifestTree.get<String>("category"),
        manifestTree.get<String>("compressedSoftwarePackageSize"),
        manifestTree.get<String>("diagnosticAddress"),
        manifestTree.get<String>("isDeltaPackage"),
        Version(manifestTree.get<String>("minUCMSupportedVersion")),
        Version(manifestTree.get<String>("maxUCMSupportedVersion")),
        manifestTree.get<String>("packagerID"),
        manifestTree.get<String>("shortName"),
        manifestTree.get<String>("typeApproval"),
        manifestTree.get<String>("ucmIdentifier"),
        manifestTree.get<String>("uncompressedSoftwarePackageSize"),
        manifestTree.get<String>("uuid"));
}

ActionType SoftwarePackageManifestParser::ParseActionType(const boost::property_tree::ptree& manifest)
{
    String actionTypeString = manifest.get<String>("actionType");

    if (actionTypeString == "Update") {
        return ActionType::kUpdate;
    }
    if (actionTypeString == "Install") {
        return ActionType::kInstall;
    }
    if (actionTypeString == "Remove") {
        return ActionType::kRemove;
    }

    throw exception::InvalidManifestException(actionTypeString + " is not a valid actionType value");
}

}  // namespace pkgmgr
}  // namespace ucm
}  // namespace ara
