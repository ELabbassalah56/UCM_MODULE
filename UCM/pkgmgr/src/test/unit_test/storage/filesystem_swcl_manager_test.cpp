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

#include "storage/filesystem_swcl_manager.h"
#include "parsing/software_package.h"
#include "test_helper.h"
#include "filesystem.h"

using boost::property_tree::ptree;
using boost::property_tree::write_json;

namespace ara
{
namespace ucm
{
namespace pkgmgr
{

class FilesystemSWCLManagerTestFixture : public ::testing::Test
{

public:
    std::unique_ptr<SWCLManager> manager_;
    Filesystem fs_;
    std::unique_ptr<SoftwareCluster> swcl1_;
    std::unique_ptr<SoftwareCluster> swcl2_;
    std::unique_ptr<SoftwarePackageManifest> swPackageManifest1_;
    std::unique_ptr<SoftwarePackageManifest> swPackageManifest2_;

    void SetUp()
    {
        manager_ = std::make_unique<FileSystemSWCLManager>(kSwclDir);
        CreateSWCLsFolderStructure();

        swPackageManifest1_ = std::make_unique<SoftwarePackageManifest>(ActionType::kInstall,
            "the activationAction",
            "the category",
            "the compressedSoftwarePackageSize",
            "the diagnosticAddress",
            "the isDeltaPackage",
            Version("1.0"),
            Version("1.0"),
            "the packagerID",
            "the shortName1",
            "the typeApproval",
            "the ucmIdentifier",
            "the uncompressedSoftwarePackageSize",
            "the uuid");
        SoftwareClusterManifest temp_1("category",
            "changes",
            "license",
            Version("1.0"),
            "shortName1",
            "uuid",
            "vendorID",
            Version("1.5"),
            {"dependentSoftwareClusterA", "dependentSoftwareClusterB", "dependentSoftwareClusterC"});
        swcl1_ = std::make_unique<SoftwareCluster>(temp_1, "");

        swPackageManifest2_ = std::make_unique<SoftwarePackageManifest>(ActionType::kRemove,
            "the activationAction",
            "the category",
            "the compressedSoftwarePackageSize",
            "the diagnosticAddress",
            "the isDeltaPackage",
            Version("1.0"),
            Version("1.0"),
            "the packagerID",
            "the shortName2",
            "the typeApproval",
            "the ucmIdentifier",
            "the uncompressedSoftwarePackageSize",
            "the uuid");
        SoftwareClusterManifest temp_2("category",
            "changes",
            "license",
            Version("1.0"),
            "shortName2",
            "uuid",
            "vendorID",
            Version("2.5"),
            {"dependentSoftwareClusterA", "dependentSoftwareClusterB", "dependentSoftwareClusterC"});
        swcl2_ = std::make_unique<SoftwareCluster>(temp_2, "");
    }

    void TearDown()
    {
        RemoveSWCLsFolderStructure();
    }
};

TEST_F(FilesystemSWCLManagerTestFixture, GetPresentSWCLs)
{
    ara::core::Vector<SoftwareCluster> presentSwcls = manager_->GetPresentSWCLs();

    ASSERT_THAT(presentSwcls.size(), 3);
    ASSERT_TRUE(std::all_of(presentSwcls.begin(), presentSwcls.end(), [](SoftwareCluster const& sc) {
        return sc.GetSwclState() == SwClusterStateType::kPresent;
    }));
}

TEST_F(FilesystemSWCLManagerTestFixture, GetSWCLsForActivation)
{
    auto swcls = manager_->GetSWCLsForActivation();
    ASSERT_THAT(swcls.size(), 3);
}

TEST_F(FilesystemSWCLManagerTestFixture, AddSWCLChangeInfo)
{
    ara::core::String source_dir = kTestOptPath + "-source";
    SoftwarePackageMock swPackage1(*swPackageManifest1_, source_dir, *swcl1_);
    SoftwarePackageMock swPackage2(*swPackageManifest2_, source_dir, *swcl2_);

    manager_->AddSWCLChangeInfo(swPackage1);
    manager_->AddSWCLChangeInfo(swPackage2);

    auto result = manager_->GetSWCLsChangeInfo();

    ASSERT_THAT(result.size(), 2);

    ASSERT_THAT(result[0].Name, "shortName1");
    ASSERT_THAT(result[0].Version, "1.5");
    ASSERT_THAT(result[0].State, SwClusterStateType::kAdded);

    ASSERT_THAT(result[1].Name, "shortName2");
    ASSERT_THAT(result[1].Version, "2.5");
    ASSERT_THAT(result[1].State, SwClusterStateType::kRemoved);

    manager_->ResetSWCLChangeInfo();

    result = manager_->GetSWCLsChangeInfo();

    ASSERT_THAT(result.size(), 0);
}

}  // namespace pkgmgr
}  // namespace ucm
}  // namespace ara
