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
#include "parsing/software_cluster_manifest.h"
#include "test_helper.h"
#include "manifest_samples.h"
#include "storage/remove_action.h"

using ::testing::Eq;

namespace ara
{
namespace ucm
{
namespace pkgmgr
{

class RemoveActionTestFixture : public ::testing::Test
{

public:
    const ara::core::String swpUnpackedPath = "/usr/var/apdtest/ucm/unpackedSwpackageA";
    ActionType uninstallType_ = ActionType::kRemove;
    const ara::core::String target_dir = kSwclDir + "/" + "swclA" + "/" + "1.0";
    std::unique_ptr<SoftwarePackageManifest> packageManifest_;
    std::unique_ptr<SoftwareCluster> swCluster_;

    Filesystem fs_;

    void SetUp()
    {
        packageManifest_ = std::make_unique<SoftwarePackageManifest>(uninstallType_,
            "the activationAction",
            "the category",
            "the compressedSoftwarePackageSize",
            "the diagnosticAddress",
            "the isDeltaPackage",
            Version("1.0.0"),
            Version("1.0.0"),
            "the packagerID",
            "swcl0",
            "the typeApproval",
            "the ucmIdentifier",
            "the uncompressedSoftwarePackageSize",
            "the uuid");
        SoftwareClusterManifest swclManifest_("category",
            "changes",
            "license",
            Version("0.5.0"),
            "swcl0",
            "uuid",
            "vendorID",
            Version("1.0.0"),
            {"dependentSoftwareClusterA", "dependentSoftwareClusterB", "dependentSoftwareClusterC"});
        swCluster_ = std::make_unique<SoftwareCluster>(swclManifest_, "");
    }

    void TearDown()
    {}
};

TEST_F(RemoveActionTestFixture, RemovePreviouslyInstalledSwPkgDir)
{
    ASSERT_TRUE(fs_.DoesDirectoryExist(kTestOptPath));
    fs_.CopyDirectory(kTestOptPath, swpUnpackedPath);

    SoftwarePackageMock sp(*packageManifest_, swpUnpackedPath, *swCluster_);

    // install the SWCL prior to remove it
    InstallAction act(target_dir, sp);

    auto result = act.Execute();

    ASSERT_THAT(result.HasValue(), true);
    ASSERT_TRUE(fs_.DoesDirectoryExist(target_dir));

    // remove the installed SWCL
    RemoveAction ra_(target_dir, sp);

    ASSERT_TRUE(fs_.DoesDirectoryExist(swpUnpackedPath));
    ra_.Execute();

    ASSERT_TRUE(fs_.DoesDirectoryExist(swpUnpackedPath));
    ASSERT_TRUE(fs_.DoesDirectoryExist(target_dir));
    ra_.CommitChanges();
    ASSERT_FALSE(fs_.DoesDirectoryExist(target_dir));
    // as the remove action has been finalised, the extraction path shall be cleaned-up
    ASSERT_FALSE(fs_.DoesDirectoryExist(swpUnpackedPath));

    // Cleanup
    fs_.RemoveDirectory(swpUnpackedPath);
    fs_.RemoveDirectory(target_dir);
    fs_.RemoveDirectory(kTestOptPath + "/" + "updateDir");
}

TEST_F(RemoveActionTestFixture, RevertRemoveOfPreviouslyInstalledSwPkgDir)
{
    ASSERT_TRUE(fs_.DoesDirectoryExist(kTestOptPath));
    fs_.CopyDirectory(kTestOptPath, swpUnpackedPath);

    SoftwarePackageMock sp(*packageManifest_, swpUnpackedPath, *swCluster_);

    // install the SWCL prior to remove it
    InstallAction act(target_dir, sp);

    auto result = act.Execute();

    ASSERT_THAT(result.HasValue(), true);
    ASSERT_TRUE(fs_.DoesDirectoryExist(target_dir));

    // remove the installed SWCL
    RemoveAction ra_(target_dir, sp);

    ASSERT_TRUE(fs_.DoesDirectoryExist(swpUnpackedPath));
    ra_.Execute();

    ASSERT_TRUE(fs_.DoesDirectoryExist(swpUnpackedPath));
    ASSERT_TRUE(fs_.DoesDirectoryExist(target_dir));

    // finally, I don't want to remove the installed SWCL
    ra_.RevertChanges();
    ASSERT_TRUE(fs_.DoesDirectoryExist(target_dir));
    // as the remove action has been aborted, the extraction path shall be cleaned-up
    ASSERT_FALSE(fs_.DoesDirectoryExist(swpUnpackedPath));

    // Cleanup
    fs_.RemoveDirectory(swpUnpackedPath);
    fs_.RemoveDirectory(target_dir);
    fs_.RemoveDirectory(kTestOptPath + "/" + "updateDir");
}

TEST_F(RemoveActionTestFixture, RemovePreviouslyInstalledSwPkgDirFailsAfterRevert)
{
    ASSERT_TRUE(fs_.DoesDirectoryExist(kTestOptPath));
    fs_.CopyDirectory(kTestOptPath, swpUnpackedPath);

    SoftwarePackageMock sp(*packageManifest_, swpUnpackedPath, *swCluster_);

    // install the SWCL prior to remove it
    InstallAction act(target_dir, sp);

    auto result = act.Execute();

    ASSERT_THAT(result.HasValue(), true);
    ASSERT_TRUE(fs_.DoesDirectoryExist(target_dir));

    // remove the installed SWCL
    RemoveAction ra_(target_dir, sp);

    ASSERT_TRUE(fs_.DoesDirectoryExist(swpUnpackedPath));
    ra_.Execute();

    ASSERT_TRUE(fs_.DoesDirectoryExist(swpUnpackedPath));
    ASSERT_TRUE(fs_.DoesDirectoryExist(target_dir));

    // finally, I don't want to remove the installed SWCL
    ra_.RevertChanges();
    ASSERT_TRUE(fs_.DoesDirectoryExist(target_dir));
    // as the remove action has been aborted, the extraction path shall be cleaned-up
    ASSERT_FALSE(fs_.DoesDirectoryExist(swpUnpackedPath));

    // Commit shall be rejected as Revert has been called previously
    ra_.CommitChanges();
    ASSERT_TRUE(fs_.DoesDirectoryExist(target_dir));

    // Cleanup
    fs_.RemoveDirectory(swpUnpackedPath);
    fs_.RemoveDirectory(target_dir);
    fs_.RemoveDirectory(kTestOptPath + "/" + "updateDir");
}

}  // namespace pkgmgr
}  // namespace ucm
}  // namespace ara
