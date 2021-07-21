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

#include "storage/update_action.h"
#include "parsing/software_package.h"
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
using ::testing::Eq;

class UpdateActionTestFixture : public ::testing::Test
{

public:
    const ara::core::String target_dir
        = kSwclDir + "/swcl_a/2_0_0_0";  /// usr/var/apdtest/ucm/opt/updateDir/swcls/swcl_a/2_0_0_0
    const ara::core::String target_dir_other
        = kSwclDir + "/swcl_b/2_0_0_0";  /// usr/var/apdtest/ucm/opt/updateDir/swcls/swcl_b/2_0_0_0
    const ara::core::String oldVersion = kSwclDir + "/swcl_a/1_0_0_0";
    const ara::core::String not_installed_swcl = kSwclDir + "/swcl_d/2_0_0_0";

    std::unique_ptr<SoftwareCluster> unusedSwcl_;
    std::unique_ptr<SoftwarePackageManifest> unusedManifest_;

    Filesystem fs_;

    void SetUp()
    {
        unusedManifest_ = std::make_unique<SoftwarePackageManifest>(ActionType::kInstall,
            "the activationAction",
            "the category",
            "the compressedSoftwarePackageSize",
            "the diagnosticAddress",
            "the isDeltaPackage",
            Version("1.0.0"),
            Version("1.0.0"),
            "the packagerID",
            "the shortName",
            "the typeApproval",
            "the ucmIdentifier",
            "the uncompressedSoftwarePackageSize",
            "the uuid");
        SoftwareClusterManifest temp_("category",
            "changes",
            "license",
            Version("0.5.0"),
            "shortName",
            "uuid",
            "vendorID",
            Version("1.0.0"),
            {"dependentSoftwareClusterA", "dependentSoftwareClusterB", "dependentSoftwareClusterC"});
        unusedSwcl_ = std::make_unique<SoftwareCluster>(temp_, "");

        CreateSWCLsFolderStructure();
    }

    void TearDown()
    {
        RemoveSWCLsFolderStructure();
    }
};

TEST_F(UpdateActionTestFixture, ExecutePositive)
{
    // make a copy of our test data so we do not touch original test data
    ara::core::String source_dir = kTestOptPath + "-source";
    ASSERT_TRUE(fs_.CopyDirectory(kTestOptPath, source_dir));

    SoftwarePackageMock sp(*unusedManifest_, source_dir, *unusedSwcl_);
    UpdateAction act(target_dir, sp);

    auto result = act.Execute();

    ASSERT_THAT(result.HasValue(), true);
    ASSERT_TRUE(fs_.DoesDirectoryExist(target_dir));

    // Cleanup
    fs_.RemoveDirectory(target_dir);
    fs_.RemoveDirectory(kTestOptPath + "/" + "updateDir");
    fs_.RemoveDirectory(source_dir);
}

TEST_F(UpdateActionTestFixture, ExecuteNegativeSwclVersionAlreadyInstalled)
{
    // make a copy of our test data so we do not touch original test data
    ara::core::String source_dir = kTestOptPath + "-source";
    ASSERT_TRUE(fs_.CopyDirectory(kTestOptPath, source_dir));

    SoftwarePackageMock sp(*unusedManifest_, source_dir, *unusedSwcl_);
    UpdateAction act(oldVersion, sp);

    auto result = act.Execute();

    ASSERT_THAT(result.HasValue(), false);
    ASSERT_TRUE(fs_.DoesDirectoryExist(oldVersion));

    // Cleanup
    fs_.RemoveDirectory(target_dir);
    fs_.RemoveDirectory(kTestOptPath + "/" + "updateDir");
    fs_.RemoveDirectory(source_dir);
}

TEST_F(UpdateActionTestFixture, ExecuteNegativeSwclNeverInstalledBefore)
{
    // make a copy of our test data so we do not touch original test data
    ara::core::String source_dir = kTestOptPath + "-source";
    ASSERT_TRUE(fs_.CopyDirectory(kTestOptPath, source_dir));

    SoftwarePackageMock sp(*unusedManifest_, source_dir, *unusedSwcl_);
    UpdateAction act(not_installed_swcl, sp);

    auto result = act.Execute();

    ASSERT_THAT(result.HasValue(), false);
    ASSERT_FALSE(fs_.DoesDirectoryExist(not_installed_swcl));

    // Cleanup
    fs_.RemoveDirectory(target_dir);
    fs_.RemoveDirectory(kTestOptPath + "/" + "updateDir");
    fs_.RemoveDirectory(source_dir);
}

TEST_F(UpdateActionTestFixture, CommitPositive)
{
    // make a copy of our test data so we do not touch original test data
    ara::core::String source_dir = kTestOptPath + "-source";
    ASSERT_TRUE(fs_.CopyDirectory(kTestOptPath, source_dir));

    SoftwarePackageMock sp(*unusedManifest_, source_dir, *unusedSwcl_);
    UpdateAction act(target_dir, sp);

    auto result = act.Execute();

    ASSERT_THAT(result.HasValue(), true);
    ASSERT_TRUE(fs_.DoesDirectoryExist(target_dir));

    auto resultFinal = act.CommitChanges();
    ASSERT_THAT(resultFinal.HasValue(), true);
    ASSERT_TRUE(fs_.DoesDirectoryExist(target_dir));
    ASSERT_FALSE(fs_.DoesDirectoryExist(oldVersion));

    // Cleanup
    fs_.RemoveDirectory(target_dir);
    fs_.RemoveDirectory(kTestOptPath + "/" + "updateDir");
    fs_.RemoveDirectory(source_dir);
}

TEST_F(UpdateActionTestFixture, CommitNegative)
{
    // make a copy of our test data so we do not touch original test data
    ara::core::String source_dir = kTestOptPath + "-source";
    ASSERT_TRUE(fs_.CopyDirectory(kTestOptPath, source_dir));

    SoftwarePackageMock sp(*unusedManifest_, source_dir, *unusedSwcl_);
    UpdateAction act(target_dir, sp);

    auto result = act.Execute();
    ASSERT_THAT(result.HasValue(), true);
    ASSERT_TRUE(fs_.DoesDirectoryExist(target_dir));

    fs_.RemoveDirectory(target_dir);
    ASSERT_FALSE(fs_.DoesDirectoryExist(target_dir));

    auto resultFinal = act.CommitChanges();
    ASSERT_THAT(resultFinal.HasValue(), false);
    ASSERT_TRUE(fs_.DoesDirectoryExist(oldVersion));

    // Cleanup
    fs_.RemoveDirectory(target_dir);
    fs_.RemoveDirectory(kTestOptPath + "/" + "updateDir");
    fs_.RemoveDirectory(source_dir);
}

TEST_F(UpdateActionTestFixture, RevertPositive)
{
    // make a copy of our test data so we do not touch original test data
    ara::core::String source_dir = kTestOptPath + "-source";
    ASSERT_TRUE(fs_.CopyDirectory(kTestOptPath, source_dir));

    SoftwarePackageMock sp(*unusedManifest_, source_dir, *unusedSwcl_);
    UpdateAction act(target_dir, sp);

    auto result = act.Execute();
    ASSERT_THAT(result.HasValue(), true);
    ASSERT_TRUE(fs_.DoesDirectoryExist(target_dir));

    auto resultFinal = act.RevertChanges();
    ASSERT_THAT(resultFinal.HasValue(), true);
    ASSERT_FALSE(fs_.DoesDirectoryExist(target_dir));

    // Cleanup
    fs_.RemoveDirectory(target_dir);
    fs_.RemoveDirectory(kTestOptPath + "/" + "updateDir");
    fs_.RemoveDirectory(source_dir);
}

TEST_F(UpdateActionTestFixture, RevertNegativeVersionAlreadyRemoved)
{
    // make a copy of our test data so we do not touch original test data
    ara::core::String source_dir = kTestOptPath + "-source";
    ASSERT_TRUE(fs_.CopyDirectory(kTestOptPath, source_dir));

    SoftwarePackageMock sp(*unusedManifest_, source_dir, *unusedSwcl_);
    UpdateAction act(target_dir, sp);

    auto result = act.Execute();
    ASSERT_THAT(result.HasValue(), true);
    ASSERT_TRUE(fs_.DoesDirectoryExist(target_dir));

    fs_.RemoveDirectory(target_dir);
    ASSERT_FALSE(fs_.DoesDirectoryExist(target_dir));

    auto resultFinal = act.RevertChanges();
    ASSERT_THAT(resultFinal.HasValue(), false);

    // Cleanup
    fs_.RemoveDirectory(target_dir);
    fs_.RemoveDirectory(kTestOptPath + "/" + "updateDir");
    fs_.RemoveDirectory(source_dir);
}

TEST_F(UpdateActionTestFixture, RevertNegativePreviousExecuteFailed)
{
    // make a copy of our test data so we do not touch original test data
    ara::core::String source_dir = kTestOptPath + "-source";
    ASSERT_TRUE(fs_.CopyDirectory(kTestOptPath, source_dir));

    SoftwarePackageMock sp(*unusedManifest_, source_dir, *unusedSwcl_);
    UpdateAction action(oldVersion, sp);

    auto result = action.Execute();
    ASSERT_THAT(result.HasValue(), false);

    auto resultFinal = action.RevertChanges();
    ASSERT_THAT(resultFinal.HasValue(), false);

    // Cleanup
    fs_.RemoveDirectory(target_dir);
    fs_.RemoveDirectory(kTestOptPath + "/" + "updateDir");
    fs_.RemoveDirectory(source_dir);
}

TEST_F(UpdateActionTestFixture, RevertNegativePreviousExecuteNotCalled)
{
    // make a copy of our test data so we do not touch original test data
    ara::core::String source_dir = kTestOptPath + "-source";
    ASSERT_TRUE(fs_.CopyDirectory(kTestOptPath, source_dir));

    SoftwarePackageMock sp(*unusedManifest_, source_dir, *unusedSwcl_);
    UpdateAction action_1(target_dir, sp);
    UpdateAction action_2(target_dir_other, sp);

    auto result = action_1.Execute();
    ASSERT_THAT(result.HasValue(), true);

    auto resultFinal = action_2.RevertChanges();
    ASSERT_THAT(resultFinal.HasValue(), false);

    // Cleanup
    fs_.RemoveDirectory(target_dir);
    fs_.RemoveDirectory(target_dir_other);
    fs_.RemoveDirectory(kTestOptPath + "/" + "updateDir");
    fs_.RemoveDirectory(source_dir);
}

TEST_F(UpdateActionTestFixture, CommitNegativePreviousExecuteNotCalled)
{
    // make a copy of our test data so we do not touch original test data
    ara::core::String source_dir = kTestOptPath + "-source";
    ASSERT_TRUE(fs_.CopyDirectory(kTestOptPath, source_dir));

    SoftwarePackageMock sp(*unusedManifest_, source_dir, *unusedSwcl_);
    UpdateAction action_1(target_dir, sp);
    UpdateAction action_2(target_dir_other, sp);

    auto result = action_1.Execute();
    ASSERT_THAT(result.HasValue(), true);

    auto resultFinal = action_2.CommitChanges();
    ASSERT_THAT(resultFinal.HasValue(), false);

    // Cleanup
    fs_.RemoveDirectory(target_dir);
    fs_.RemoveDirectory(target_dir_other);
    fs_.RemoveDirectory(kTestOptPath + "/" + "updateDir");
    fs_.RemoveDirectory(source_dir);
}

}  // namespace pkgmgr
}  // namespace ucm
}  // namespace ara
