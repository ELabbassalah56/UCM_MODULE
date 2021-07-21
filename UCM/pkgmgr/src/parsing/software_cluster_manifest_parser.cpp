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

#include "parsing/software_cluster_manifest_parser.h"

#include "boost/property_tree/ptree.hpp"
#include "boost/property_tree/detail/file_parser_error.hpp"
#include "boost/property_tree/json_parser.hpp"

#include "parsing/version.h"
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

static ara::log::Logger& logger()
{
    static ara::log::Logger& loggerInstance = ara::log::CreateLogger("SCMP", "SoftwareClusterManifestParser context");
    return loggerInstance;
}

std::unique_ptr<SoftwareClusterManifest> SoftwareClusterManifestParser::Parse(
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

std::unique_ptr<SoftwareClusterManifest> SoftwareClusterManifestParser::ParseFromPtree(
    const boost::property_tree::ptree& manifestTree)
{
    Version previousVersion(0, 0);
    try {
        previousVersion = Version(manifestTree.get<std::string>("previousVersion"));
    } catch (const std::invalid_argument& a) {
        String message = helper::CreateErrorMessage("wrong previousVersion", a.what());
        logger().LogError() << message;
        throw exception::InvalidManifestException(message);
    }
    Version version(0, 0);
    try {
        version = Version(manifestTree.get<std::string>("version"));
    } catch (const std::invalid_argument& a) {
        String message = helper::CreateErrorMessage("wrong version", a.what());
        logger().LogError() << message;
        throw exception::InvalidManifestException(message);
    }
    return std::make_unique<SoftwareClusterManifest>(
        // TODO: clarify why ptree::get<std::string> works and ptree::get<ara::core::String> fails?
        manifestTree.get<std::string>("category"),
        manifestTree.get<std::string>("changes"),
        manifestTree.get<std::string>("license"),
        previousVersion,
        manifestTree.get<std::string>("shortName"),
        manifestTree.get<std::string>("uuid"),
        manifestTree.get<std::string>("vendorID"),
        version,
        ara::core::Vector<String>());
}

const ara::core::Vector<String> SoftwareClusterManifestParser::ParseStringArray(const ptree& manifestTree,
    const String& key)
{
    ara::core::Vector<String> names;
    for (auto& application : manifestTree.get_child(key.c_str())) {
        names.push_back(application.second.get_value<String>());
    }
    return names;
}

}  // namespace pkgmgr
}  // namespace ucm
}  // namespace ara
