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

#include "parsing/software_package.h"
#include "parsing/software_package_manifest.h"

#include "parsing/software_cluster_manifest.h"
#include "parsing/software_cluster_manifest_parser.h"
#include "parsing/software_cluster.h"

#include "storage/reversible_action.h"
#include "storage/install_action.h"
#include "storage/remove_action.h"
#include "storage/action_generator.h"

#include "test_helper.h"
#include "filesystem.h"

#include <cstdio>
#include <fstream>
#include <cstdint>

namespace ara
{
namespace ucm
{
namespace pkgmgr
{

using String = ara::core::String;
class ActionGeneratorTestFixture : public ::testing::Test
{
public:
    std::unique_ptr<SoftwarePackageManifest> packageManifest_;
    std::unique_ptr<SoftwareCluster> swCluster_;
    Filesystem fs_;

    void SetUp()
    {
        packageManifest_ = std::make_unique<SoftwarePackageManifest>(ActionType::kInstall,
            "the activationAction",
            "the category",
            "the compressedSoftwarePackageSize",
            "the diagnosticAddress",
            "the isDeltaPackage",
            Version("1.0.0"),
            Version("1.0.0"),
            "the packagerID",
            "swclDiag",
            "the typeApproval",
            "the ucmIdentifier",
            "the uncompressedSoftwarePackageSize",
            "the uuid");
        SoftwareClusterManifest swclManifest_("the category",
            "changes",
            "license",
            Version("0.5.0"),
            "swclA",
            "the uuid",
            "the vendorID",
            Version("1.0.0"),
            {"dependentSoftwareClusterA", "dependentSoftwareClusterB", "dependentSoftwareClusterC"});
        swCluster_ = std::make_unique<SoftwareCluster>(swclManifest_, "");
    }

    void TearDown()
    {}
};

TEST_F(ActionGeneratorTestFixture, CreateInstallAction)
{
    // make a copy of our test data so we do not touch original test data
    ara::core::String source_dir = kTestOptPath + "-source";
    ASSERT_TRUE(fs_.CopyDirectory(kTestOptPath, source_dir));

    ara::core::String targetPathFinalized = kSwclDir;
    SoftwarePackageMock swPackage(*packageManifest_, source_dir, *swCluster_);

    EXPECT_CALL(swPackage, GetManifest()).Times(1);

    std::unique_ptr<ReversibleAction> ReversibleAction1;
    ReversibleAction1 = ActionGenerator()(targetPathFinalized, swPackage);

    auto result = ReversibleAction1->Execute();

    ASSERT_THAT(result.HasValue(), true);
    ASSERT_TRUE(fs_.DoesDirectoryExist(targetPathFinalized + "/" + "swclA"));
    ASSERT_TRUE(fs_.DoesDirectoryExist(targetPathFinalized + "/" + "swclA" + "/" + "1.0.0"));

    // Cleanup
    fs_.RemoveDirectory(targetPathFinalized);
    fs_.RemoveDirectory(targetPathFinalized + "/" + "swclA" + "/" + "1.0.0");
    fs_.RemoveDirectory(kTestOptPath + "/" + "updateDir");
    fs_.RemoveDirectory(source_dir);
}

TEST_F(ActionGeneratorTestFixture, CreateInstallActionAndRevert)
{
    // make a copy of our test data so we do not touch original test data
    ara::core::String source_dir = kTestOptPath + "-source";
    ASSERT_TRUE(fs_.CopyDirectory(kTestOptPath, source_dir));

    ara::core::String targetPathFinalized = kSwclDir;
    SoftwarePackageMock swPackage(*packageManifest_, source_dir, *swCluster_);

    EXPECT_CALL(swPackage, GetManifest()).Times(1);

    std::unique_ptr<ReversibleAction> ReversibleAction1;
    ReversibleAction1 = ActionGenerator()(targetPathFinalized, swPackage);

    auto result = ReversibleAction1->Execute();

    ASSERT_THAT(result.HasValue(), true);
    ASSERT_TRUE(fs_.DoesDirectoryExist(targetPathFinalized + "/" + "swclA"));
    ASSERT_TRUE(fs_.DoesDirectoryExist(targetPathFinalized + "/" + "swclA" + "/" + "1.0.0"));

    auto resultRevert = ReversibleAction1->RevertChanges();
    ASSERT_THAT(result.HasValue(), true);
    ASSERT_TRUE(fs_.DoesDirectoryExist(targetPathFinalized + "/" + "swclA"));
    ASSERT_FALSE(fs_.DoesDirectoryExist(targetPathFinalized + "/" + "swclA" + "/" + "1.0.0"));

    // Cleanup
    fs_.RemoveDirectory(targetPathFinalized);
    fs_.RemoveDirectory(targetPathFinalized + "/" + "swclA" + "/" + "1.0.0");
    fs_.RemoveDirectory(kTestOptPath + "/" + "updateDir");
    fs_.RemoveDirectory(source_dir);
}

TEST_F(ActionGeneratorTestFixture, CreateInstallActionFailedAndTryToRevertIt)
{
    // make a copy of our test data so we do not touch original test data
    ara::core::String source_dir = kTestOptPath + "-source";
    ASSERT_TRUE(fs_.CopyDirectory(kTestOptPath, source_dir));

    ara::core::String targetPathFinalized = kSwclDir;
    SoftwarePackageMock swPackage(*packageManifest_, source_dir + "-non-exists/", *swCluster_);

    EXPECT_CALL(swPackage, GetManifest()).Times(1);

    std::unique_ptr<ReversibleAction> ReversibleAction1;
    ReversibleAction1 = ActionGenerator()(targetPathFinalized, swPackage);

    auto result = ReversibleAction1->Execute();

    ASSERT_THAT(result.HasValue(), false);
    ASSERT_FALSE(fs_.DoesDirectoryExist(targetPathFinalized + "/" + "swclA" + "/" + "1.0.0"));

    auto resultRevert = ReversibleAction1->RevertChanges();
    ASSERT_THAT(result.HasValue(), false);

    // Cleanup
    fs_.RemoveDirectory(targetPathFinalized + "/" + "swclA" + "/" + "1.0.0");
    fs_.RemoveDirectory(kTestOptPath + "/" + "updateDir");
    fs_.RemoveDirectory(source_dir);
}

}  // namespace pkgmgr
}  // namespace ucm
}  // namespace ara
