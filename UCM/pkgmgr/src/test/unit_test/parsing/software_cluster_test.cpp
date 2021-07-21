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
#include "parsing/software_cluster_manifest.h"
#include "parsing/software_cluster.h"
#include "test_helper.h"
#include "manifest_samples.h"
#include "ara/ucm/pkgmgr/impl_type_actiontype.h"

using ::testing::Eq;

namespace ara
{
namespace ucm
{
namespace pkgmgr
{

class SoftwareClusterTestFixture : public ::testing::Test
{
public:
    SoftwareClusterTestFixture()
        : swclManifest_("the category",
              "the changes",
              "the license",
              Version("0.5"),
              "swcl0",
              "the uuid",
              "the vendorID",
              Version("1.0"),
              {"dependentSoftwareClusterA", "dependentSoftwareClusterB", "dependentSoftwareClusterC"})
        , swcl_(swclManifest_, manifestPath_)
    {}

    const ara::core::String manifestPath_{"ucm/temp/buffer"};
    const ara::core::String destinationPath_{"UCM/SWCL_DIR"};
    SoftwareClusterManifest swclManifest_;
    SoftwareCluster swcl_;
};

TEST_F(SoftwareClusterTestFixture, GetManifestPath)
{
    const ara::core::String path = swcl_.GetManifestPath();

    ASSERT_THAT(path, Eq(manifestPath_));
}

TEST_F(SoftwareClusterTestFixture, GetVersionFromManifest)
{
    Version swclVersion = swcl_.GetSwclManifest().GetVersion();
    String swclVersionString = swclVersion.ToString();

    // Usage of EXPECT_EQ because ASSERT_EQ seems not to work with user-defined types like ara::core::String
    ASSERT_EQ(swclVersionString, "1.0");
}

TEST_F(SoftwareClusterTestFixture, GetSwclNameFromManifest)
{
    const ara::core::String swclName = swcl_.GetSwclManifest().GetShortName();

    // Usage of EXPECT_EQ because ASSERT_EQ seems not to work with user-defined types like ara::core::String
    EXPECT_EQ(swclName, "swcl0");
}

}  // namespace pkgmgr
}  // namespace ucm
}  // namespace ara
