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

#ifndef ARA_UCM_PKGMGR_PARSING_SOFTWARE_CLUSTER_MANIFEST_PARSER_H_
#define ARA_UCM_PKGMGR_PARSING_SOFTWARE_CLUSTER_MANIFEST_PARSER_H_

#include "ara/core/string.h"
#include "ara/core/vector.h"
#include "parsing/version.h"
#include "parsing/software_cluster_manifest.h"
#include "boost/property_tree/ptree.hpp"
#include "parsing/manifest_parser.h"

namespace ara
{
namespace ucm
{
namespace pkgmgr
{
using String = ara::core::String;
/// @brief Parser for the software cluster manifest
class SoftwareClusterManifestParser : public ManifestParser<SoftwareClusterManifest>
{
public:
    SoftwareClusterManifestParser() = default;
    virtual ~SoftwareClusterManifestParser() = default;

    /// <inheritdoc>
    std::unique_ptr<SoftwareClusterManifest> Parse(const boost::property_tree::ptree& manifest) override;

private:
    /// @brief Helper method to parse manifest from ptree.
    ///
    /// @param manifest The software cluster manifest loaded as a ptree
    ///
    /// @throw ptree_error if any errors occured during accessing the ptree
    /// @returns The parsed software cluster manifest
    std::unique_ptr<SoftwareClusterManifest> ParseFromPtree(const boost::property_tree::ptree& manifest);

    /// @brief Parse the attribute that are of type string[]
    ///
    /// @param manifest The software cluster manifest
    /// @param key The key in json
    ///
    /// @throws ptree_error if an error occured when accessing the ptree object
    /// @returns The list of elements in the given array of strings
    const ara::core::Vector<String> ParseStringArray(const boost::property_tree::ptree& manifest, const String& key);
};

}  // namespace pkgmgr
}  // namespace ucm
}  // namespace ara

#endif  // ARA_UCM_PKGMGR_PARSING_SOFTWARE_CLUSTER_MANIFEST_PARSER_H_
