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
#include "test_helper.h"
#include "filesystem.h"
#include "extraction/lib_poco_zip_extractor.h"

using ::testing::Eq;
using testing::Types;

namespace ara
{
namespace ucm
{
namespace pkgmgr
{

template <class T>
std::unique_ptr<T> CreateSoftwareExtractor(Filesystem& fs);

template <>
std::unique_ptr<LibPocoZipExtractor> CreateSoftwareExtractor<LibPocoZipExtractor>(Filesystem& fs)
{
    return std::make_unique<LibPocoZipExtractor>(fs);
}

template <typename T>
class SoftwarePackageExtractorFixture : public ::testing::Test
{
public:
    SoftwarePackageExtractorFixture()
        : packageExtractor_(CreateSoftwareExtractor<T>(fs_))
    {}

    void TearDown()
    {
        packageExtractor_->Cleanup();
    }

    void CheckFilesystemForArchiveContent(const ara::core::String& extractionPath)
    {
        ara::core::String basePath = extractionPath + "/";
        EXPECT_TRUE(fs_.DoesDirectoryExist(basePath));
        EXPECT_TRUE(fs_.DoesFileExist(basePath + "/" + "SWCL_MANIFEST.json"));
        basePath = basePath + "appA/exec1";
        EXPECT_TRUE(fs_.DoesDirectoryExist(basePath + "/" + "bin"));
        EXPECT_TRUE(fs_.DoesFileExist(basePath + "/" + "bin/" + "app"));
        EXPECT_TRUE(fs_.DoesDirectoryExist(basePath + "/" + "etc"));
        EXPECT_TRUE(fs_.DoesFileExist(basePath + "/" + "etc/" + "MANIFEST.json"));
        EXPECT_TRUE(fs_.DoesDirectoryExist(basePath + "/var"));
        EXPECT_TRUE(fs_.DoesFileExist(basePath + "/" + "var/" + "kvsdatabase.json"));
    }

    const ara::core::String testPackagePath = kTestInstallSwPackagePath;
    const ara::core::String testPackageExtractionPath = kTestWorkingDir + "/swp1";
    std::unique_ptr<T> packageExtractor_;
    Filesystem fs_;
};

// The list of types we want to test.
using Implementations = Types<LibPocoZipExtractor>;
TYPED_TEST_CASE(SoftwarePackageExtractorFixture, Implementations);

TYPED_TEST(SoftwarePackageExtractorFixture, ArchiveDoesNotExist)
{
    bool result = this->packageExtractor_->Extract("/usr/nonExistingArchive.zip", this->testPackageExtractionPath);

    ASSERT_THAT(result, Eq(false));
    ASSERT_THAT(this->fs_.DoesDirectoryExist(this->testPackageExtractionPath), Eq(false));
}

TYPED_TEST(SoftwarePackageExtractorFixture, ExtractArchive)
{
    bool result = this->packageExtractor_->Extract(this->testPackagePath, this->testPackageExtractionPath);

    ASSERT_THAT(result, Eq(true));
    this->CheckFilesystemForArchiveContent(this->testPackageExtractionPath);

    this->packageExtractor_->Cleanup();
}

TYPED_TEST(SoftwarePackageExtractorFixture, Cleanup)
{
    bool result = this->packageExtractor_->Extract(this->testPackagePath, this->testPackageExtractionPath);
    ASSERT_THAT(result, Eq(true));

    this->packageExtractor_->Cleanup();
    ASSERT_THAT(this->fs_.DoesDirectoryExist(this->testPackageExtractionPath), Eq(false));
}

}  // namespace pkgmgr
}  // namespace ucm
}  // namespace ara
