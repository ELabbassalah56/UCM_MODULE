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

#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include <algorithm>

#include "boost/property_tree/ptree.hpp"
#include "parsing/software_cluster_manifest_parser.h"
#include "parsing/software_package_manifest_parser.h"
#include "parsing/software_cluster_manifest.h"
#include "test_helper.h"
#include "manifest_samples.h"
#include "ara/core/string.h"
#include "exceptions.h"

using ::testing::Eq;
using ::testing::ContainerEq;
using testing::Types;
using boost::property_tree::ptree;

namespace ara
{
namespace ucm
{
namespace pkgmgr
{

template <typename T>
class ManifestParserFixture : public ::testing::Test
{
public:
    ManifestParserFixture()
    {}

    void WriteToFile(const ara::core::String& path, const ara::core::String& data)
    {
        std::ofstream out;
        out.open(path.c_str(), std::ios::out);
        if (out.is_open()) {
            out << data;
            out.close();
        }
    }

    T parser_;
};

// The list of types we want to test.
using Implementations = Types<SoftwareClusterManifestParser, SoftwarePackageManifestParser>;
TYPED_TEST_CASE(ManifestParserFixture, Implementations);

TYPED_TEST(ManifestParserFixture, ParseFromNonExistingFile)
{
    ASSERT_THROW(this->parser_.ParseFromFile("notexistingfile.json"), exception::InvalidManifestException);
}

TYPED_TEST(ManifestParserFixture, ParseManifestWithMissingAttributes)
{
    ptree invalidManifest;
    invalidManifest.put("version", "1.0");

    ASSERT_THROW(this->parser_.Parse(invalidManifest), exception::InvalidManifestException);
}

TYPED_TEST(ManifestParserFixture, ParseManifestWithSyntaxError)
{
    ptree invalidManifest;
    invalidManifest.put("version", "1.y0");

    ASSERT_THROW(this->parser_.Parse(invalidManifest), exception::InvalidManifestException);
}

TYPED_TEST(ManifestParserFixture, ParseManifestWithInvalidJsonSyntax)
{
    const ara::core::String invalidManifest = "{ []:\"test }";
    const ara::core::String invalidManifestPath = "invalidManifest.json";
    this->WriteToFile(invalidManifestPath, invalidManifest);

    ASSERT_THROW(this->parser_.ParseFromFile(invalidManifestPath), exception::InvalidManifestException);

    std::remove(invalidManifestPath.c_str());
}

}  // namespace pkgmgr
}  // namespace ucm
}  // namespace ara
