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

#ifndef ARA_UCM_PKGMGR_PARSING_SOFTWARE_MANIFEST_PARSER_H_
#define ARA_UCM_PKGMGR_PARSING_SOFTWARE_MANIFEST_PARSER_H_

#include "ara/core/string.h"
#include "boost/property_tree/ptree.hpp"
#include "boost/property_tree/json_parser.hpp"
#include "exceptions.h"

namespace ara
{
namespace ucm
{
namespace pkgmgr
{

/// @brief Common interface for manifest parsers.
///
/// This class implements common behavior that is true for all parsers.
///
/// @tparam T The class of the parsed manifest, i.e. SoftwareClusterManifest or SoftwarePackageManifest
template <class T>
class ManifestParser
{
public:
    ManifestParser() = default;
    virtual ~ManifestParser() = default;

    /// @brief Parses a manifest given by the content of the ptree.
    ///
    /// If no exception is thrown, the provided manifest is valid.
    ///
    /// @param manifest The manifest json loaded as a ptree
    ///
    /// @throws InvalidManifestException if any errors encountered during manifest parsing
    /// @returns The parsed manifest object representing the json content
    virtual std::unique_ptr<T> Parse(const boost::property_tree::ptree& manifest) = 0;

    /// @brief Parses a manifest given by the absolute file path.
    ///
    /// If this method does not raise an InvalidManifestException then the resulting manifest
    /// object is valid.
    ///
    /// @param manifestPath The absolute path to a manifest json
    ///
    /// @throw InvalidManifestException if any errors encountered during manifest parsing
    /// @returns The parsed software cluster manifest object representing the json content
    virtual std::unique_ptr<T> ParseFromFile(const ara::core::String& manifestPath)
    {
        boost::property_tree::ptree manifestTree;

        try {
            boost::property_tree::read_json(manifestPath.c_str(), manifestTree);
        } catch (const boost::property_tree::file_parser_error& error) {
            ara::core::String msg = "Error reading manifest from filesystem at " + manifestPath + " :" + error.what();
            throw exception::InvalidManifestException(msg);
        }

        return Parse(manifestTree);
    }
};

}  // namespace pkgmgr
}  // namespace ucm
}  // namespace ara

#endif  // ARA_UCM_PKGMGR_PARSING_SOFTWARE_MANIFEST_PARSER_H_
