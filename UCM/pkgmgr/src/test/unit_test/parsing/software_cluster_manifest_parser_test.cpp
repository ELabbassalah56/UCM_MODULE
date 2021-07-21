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

#include "parsing/software_cluster_manifest_parser.h"
#include "parsing/software_cluster_manifest.h"
#include "test_helper.h"
#include "manifest_samples.h"
#include "boost/property_tree/ptree.hpp"
#include "ara/core/vector.h"
#include "ara/core/string.h"
#include <algorithm>

using ::testing::Eq;
using ::testing::ContainerEq;
using boost::property_tree::ptree;
using boost::property_tree::write_json;

namespace ara
{
namespace ucm
{
namespace pkgmgr
{
using String = ara::core::String;
class ManifestTestData
{
public:
    String name_ = "swcl0";
    String version_ = String("1.0");
};

class SoftwareClusterManifestParserFixture : public ::testing::Test
{
public:
    SoftwareClusterManifestParserFixture()
    {
        sampleManifest_ = CreateSampleManifest();
    }

    void SetUp()
    {
        WriteToFile(sampleManifestPath_, sampleManifest_);
    }

    void TearDown()
    {
        std::remove(sampleManifestPath_.c_str());
    }

    void WriteToFile(const String& path, const ptree& ptree)
    {
        std::ofstream out;
        out.open(path.c_str(), std::ios::out);
        if (out.is_open()) {
            write_json(out, ptree);
            out.close();
        }
    }

    ptree CreateSampleManifest()
    {
        return testdata::CreateSoftwareClusterManifest(testData_.name_, testData_.version_);
    }

    void CompareManifestToTestdata(SoftwareClusterManifest& manifest)
    {
        EXPECT_THAT(manifest.GetShortName(), Eq(testData_.name_));
        EXPECT_THAT(manifest.GetVersion().ToString(), Eq(testData_.version_));
    }

    SoftwareClusterManifestParser parser_;
    ManifestTestData testData_;
    ptree sampleManifest_;
    const String sampleManifestPath_ = "manifest.json";
};

TEST_F(SoftwareClusterManifestParserFixture, ParseFromPTree)
{
    std::unique_ptr<SoftwareClusterManifest> parsedManifest = parser_.Parse(sampleManifest_);

    CompareManifestToTestdata(*parsedManifest);
}

TEST_F(SoftwareClusterManifestParserFixture, ParseFromFile)
{
    std::unique_ptr<SoftwareClusterManifest> parsedManifest = parser_.ParseFromFile(sampleManifestPath_);

    CompareManifestToTestdata(*parsedManifest);
}

}  // namespace pkgmgr
}  // namespace ucm
}  // namespace ara
