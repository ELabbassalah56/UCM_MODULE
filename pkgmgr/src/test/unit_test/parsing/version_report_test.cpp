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
#include "parsing/software_package_manifest.h"
#include "parsing/software_cluster_list_builder.h"
#include "parsing/version_report_impl.h"
#include "test_helper.h"
#include "manifest_samples.h"

using ::testing::Eq;

namespace ara
{
namespace ucm
{
namespace pkgmgr
{

class VersionReportImplFixture : public ::testing::Test
{
public:
    VersionReportImplFixture()
    {}

    std::unique_ptr<SoftwareClusterListBuilderMock> builderActive_;
    std::unique_ptr<SoftwareClusterListBuilderMock> builderInactive_;
    std::unique_ptr<VersionReportImpl> versionReport_;
    // VersionReportImplMock needs to be made.

    void SetUp()
    {

        builderActive_ = std::make_unique<SoftwareClusterListBuilderMock>();
        builderInactive_ = std::make_unique<SoftwareClusterListBuilderMock>();

        SoftwareClusterMock swclActive1 = SoftwareClusterMock(
            {"the category",
                "the changes",
                "the license",
                Version("2.0.0"),
                "swcl1",
                "the uuid",
                "the vendorID",
                Version("2.3.0"),
                {"dependentSoftwareClusterA", "dependentSoftwareClusterB", "dependentSoftwareClusterC"}},
            "");

        SoftwareClusterMock swclActive2 = SoftwareClusterMock(
            {"the category",
                "the changes",
                "the license",
                Version("2.0.0"),
                "swcl2",
                "the uuid",
                "the vendorID",
                Version("2.5.0"),
                {"dependentSoftwareClusterA", "dependentSoftwareClusterB", "dependentSoftwareClusterC"}},
            "");

        SoftwareClusterMock swclInactive1 = SoftwareClusterMock(
            {"the category",
                "the changes",
                "the license",
                Version("2.0.0"),
                "swcl1",
                "the uuid",
                "the vendorID",
                Version("2.4.0"),
                {"dependentSoftwareClusterA", "dependentSoftwareClusterB", "dependentSoftwareClusterC"}},
            "");

        SoftwareClusterMock swclInactive2 = SoftwareClusterMock(
            {"the category",
                "the changes",
                "the license",
                Version("1.0.0"),
                "swcl2",
                "the uuid",
                "the vendorID",
                Version("1.4.0"),
                {"dependentSoftwareClusterA", "dependentSoftwareClusterB", "dependentSoftwareClusterC"}},
            "");

        SoftwareClusterMock swclInactive3 = SoftwareClusterMock(
            {"the category",
                "the changes",
                "the license",
                Version("1.0.0"),
                "swcl3",
                "the uuid",
                "the vendorID",
                Version("1.7.0"),
                {"dependentSoftwareClusterA", "dependentSoftwareClusterB", "dependentSoftwareClusterC"}},
            "");

        SoftwareClusterMock swclInactive4 = SoftwareClusterMock(
            {"the category",
                "the changes",
                "the license",
                Version("0.5.0"),
                "swcl4",
                "the uuid",
                "the vendorID",
                Version("1.0.0"),
                {"dependentSoftwareClusterA", "dependentSoftwareClusterB", "dependentSoftwareClusterC"}},
            "");

        swclInactive1.SetSwclState(SwClusterStateType::kUpdated);
        swclInactive2.SetSwclState(SwClusterStateType::kUpdated);
        swclInactive3.SetSwclState(SwClusterStateType::kRemoved);
        swclInactive4.SetSwclState(SwClusterStateType::kAdded);

        builderActive_->AddSwcl(swclActive1);
        builderActive_->AddSwcl(swclActive2);

        builderInactive_->AddSwcl(swclInactive1);
        builderInactive_->AddSwcl(swclInactive2);
        builderInactive_->AddSwcl(swclInactive3);
        builderInactive_->AddSwcl(swclInactive4);

        versionReport_ = std::make_unique<VersionReportImpl>(*builderActive_, *builderInactive_);
    }
    FilesystemMock fs_;
};

TEST_F(VersionReportImplFixture, ReportOnActivePartition)
{
    // get active partition contents
    ara::core::Vector<SwClusterInfoType> SwClusterInfoActive = versionReport_->GetSwClusterInfo();

    // check each software cluster's name, version, and state.
    EXPECT_EQ(SwClusterInfoActive[0].Name, "swcl1");
    EXPECT_EQ(SwClusterInfoActive[1].Name, "swcl2");
    EXPECT_EQ(SwClusterInfoActive[0].Version, "2.3.0");
    EXPECT_EQ(SwClusterInfoActive[1].Version, "2.5.0");
    EXPECT_EQ(SwClusterInfoActive[0].State, SwClusterStateType::kPresent);
    EXPECT_EQ(SwClusterInfoActive[1].State, SwClusterStateType::kPresent);
}

TEST_F(VersionReportImplFixture, ReportOnInActivePartition)
{
    // get inactive partition contents
    ara::core::Vector<SwClusterInfoType> SwClusterInfoInActive = versionReport_->GetSwClusterChangeInfo();

    // check each software cluster's name, version, and state.
    EXPECT_EQ(SwClusterInfoInActive[0].Name, "swcl1");
    EXPECT_EQ(SwClusterInfoInActive[1].Name, "swcl2");
    EXPECT_EQ(SwClusterInfoInActive[2].Name, "swcl3");
    EXPECT_EQ(SwClusterInfoInActive[3].Name, "swcl4");
    EXPECT_EQ(SwClusterInfoInActive[0].Version, "2.4.0");
    EXPECT_EQ(SwClusterInfoInActive[1].Version, "1.4.0");
    EXPECT_EQ(SwClusterInfoInActive[2].Version, "1.7.0");
    EXPECT_EQ(SwClusterInfoInActive[3].Version, "1.0.0");
    EXPECT_EQ(SwClusterInfoInActive[0].State, SwClusterStateType::kUpdated);
    EXPECT_EQ(SwClusterInfoInActive[1].State, SwClusterStateType::kUpdated);
    EXPECT_EQ(SwClusterInfoInActive[2].State, SwClusterStateType::kRemoved);
    EXPECT_EQ(SwClusterInfoInActive[3].State, SwClusterStateType::kAdded);
}

}  // namespace pkgmgr
}  // namespace ucm
}  // namespace ara
