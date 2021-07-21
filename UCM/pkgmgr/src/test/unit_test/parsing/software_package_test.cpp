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

class SoftwarePackageTestFixture : public ::testing::Test
{
public:
    SoftwarePackageTestFixture()
        : packageManifest_(installType_,
              "the activationAction",
              "the category",
              "the compressedSoftwarePackageSize",
              "the diagnosticAddress",
              "the isDeltaPackage",
              Version("1.0.0"),
              Version("1.0.0"),
              "the packagerID",
              swclName_,
              "the typeApproval",
              "the ucmIdentifier",
              "the uncompressedSoftwarePackageSize",
              "the uuid")
        , swpackage_(packageManifest_, testPath_, swcl_)
    {}

    const ara::core::String testPath_{"my/test/path"};
    const ara::core::String swclName_ = "swcl0";
    ActionType installType_ = ActionType::kInstall;
    SoftwarePackageManifest packageManifest_;
    SoftwareClusterManifest swclManifest_ = {"the category",
        "the changes",
        "the license",
        Version("0.5.0"),
        "the shortName",
        "the uuid",
        "the vendorID",
        Version("1.0.0"),
        {"dependentSoftwareClusterA", "dependentSoftwareClusterB", "dependentSoftwareClusterC"}};
    SoftwareCluster swcl_ = {swclManifest_, ""};
    SoftwarePackage swpackage_;
};

TEST_F(SoftwarePackageTestFixture, GetExtractionPath)
{
    const ara::core::String path = swpackage_.GetExtractionPath();

    ASSERT_THAT(path, Eq(testPath_));
}

TEST_F(SoftwarePackageTestFixture, GetActionType)
{
    ActionType actionType = swpackage_.GetManifest().GetActionType();

    ASSERT_THAT(actionType, Eq(installType_));
}
}  // namespace pkgmgr
}  // namespace ucm
}  // namespace ara
