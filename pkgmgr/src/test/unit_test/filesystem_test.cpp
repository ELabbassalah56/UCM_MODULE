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

#include "filesystem.h"
#include "boost/filesystem.hpp"
#include <dirent.h>
#include "test_helper.h"
#include <fstream>
#include <iostream>
#include "ara/log/logging.h"

namespace ara
{
namespace ucm
{
namespace pkgmgr
{

using ::testing::Eq;
using ::testing::Ne;
using ::testing::StrEq;
using ara::ucm::pkgmgr::kTestInstallSwPackagePath;
using ara::ucm::pkgmgr::kTestOptPath;

class FilesystemTestFixture : public ::testing::Test
{

public:
    Filesystem fs_;

    void SetUp()
    {}

    void TearDown()
    {}

    const ara::core::String existing_dir = kTestOptPath;
    const ara::core::String existing_file = kTestInstallSwPackagePath;
};

void CreateDummyFile(const ara::core::String& path)
{
    std::ofstream out;
    out.open(path.c_str(), std::ios::out);
    if (out.is_open()) {
        out << "Test";
        out.close();
    }
}

TEST_F(FilesystemTestFixture, DoesDirectoryExist_Positive)
{
    ASSERT_TRUE(fs_.DoesDirectoryExist(existing_dir));
}

TEST_F(FilesystemTestFixture, DoesDirectoryExist_Negative)
{
    ASSERT_FALSE(fs_.DoesDirectoryExist(existing_dir + "123"));
}

TEST_F(FilesystemTestFixture, DoesDirectoryExist_File)
{
    ASSERT_FALSE(fs_.DoesDirectoryExist(existing_file));
}

TEST_F(FilesystemTestFixture, DoesFileExist_Positive)
{
    ASSERT_TRUE(fs_.DoesFileExist(existing_file));
}

TEST_F(FilesystemTestFixture, DoesFileExist_Negative)
{
    ASSERT_FALSE(fs_.DoesFileExist(existing_file + "123"));
}

TEST_F(FilesystemTestFixture, DoesFileExist_Directory)
{
    ASSERT_FALSE(fs_.DoesFileExist(existing_dir));
}

TEST_F(FilesystemTestFixture, CopyFile_rename)
{
    ara::core::String sourcePath = kTestWorkingDir + "/temp-source.txt";
    ara::core::String targetPath = kTestWorkingDir + "/temp-target.txt";
    CreateDummyFile(sourcePath);

    ASSERT_TRUE(!fs_.DoesFileExist(targetPath));
    ASSERT_TRUE(fs_.CopyFile(sourcePath, targetPath));
    ASSERT_TRUE(fs_.DoesFileExist(targetPath));
    ASSERT_TRUE(fs_.DoesFileExist(sourcePath));

    std::remove(targetPath.c_str());
    std::remove(sourcePath.c_str());
}

TEST_F(FilesystemTestFixture, MoveDirectory)
{
    // make a copy of our test data so we do not touch original test data
    ara::core::String sourceDir = existing_dir + "-source";
    ASSERT_TRUE(fs_.CopyDirectory(existing_dir, sourceDir));
    const ara::core::String targetDir = sourceDir + "-temp";
    ASSERT_FALSE(fs_.DoesDirectoryExist(targetDir));

    ASSERT_TRUE(fs_.MoveDirectory(sourceDir, targetDir));

    for (boost::filesystem::recursive_directory_iterator end, dir(existing_dir.c_str()); dir != end; ++dir) {
        auto filename = dir->path().lexically_relative(existing_dir.c_str());
        auto targetpath = targetDir + "/" + filename.string();
        auto sourcepath = sourceDir + "/" + filename.string();

        // each entry is either a directory or a file
        ASSERT_TRUE(fs_.DoesDirectoryExist(targetpath) || fs_.DoesFileExist(targetpath));
        // each file and directory does no longer exist in its original location
        ASSERT_FALSE(fs_.DoesDirectoryExist(sourcepath) || fs_.DoesFileExist(sourcepath));
    }

    // cleanup
    fs_.RemoveDirectory(targetDir);
    fs_.RemoveDirectory(sourceDir);
}

TEST_F(FilesystemTestFixture, CopyDirectory)
{
    const ara::core::String targetDir = existing_dir + "-temp";
    ASSERT_FALSE(fs_.DoesDirectoryExist(targetDir));

    ASSERT_TRUE(fs_.CopyDirectory(existing_dir, targetDir));

    for (boost::filesystem::recursive_directory_iterator end, dir(existing_dir.c_str()); dir != end; ++dir) {
        auto filename = dir->path().lexically_relative(existing_dir.c_str());
        auto targetpath = targetDir + "/" + filename.string();

        // each entry is either a directory or a file
        ASSERT_TRUE(fs_.DoesDirectoryExist(targetpath) || fs_.DoesFileExist(targetpath));
    }

    // cleanup
    fs_.RemoveDirectory(targetDir);
}

TEST_F(FilesystemTestFixture, RemoveDirectory_Negative)
{
    const ara::core::String kSuffix = "-nonexistent";
    auto nonExistingDir = existing_dir + kSuffix;

    ASSERT_FALSE(fs_.RemoveDirectory(nonExistingDir));
}

TEST_F(FilesystemTestFixture, RemoveDirectory_Positive)
{
    const ara::core::String kSuffix = "-nonexistent";
    auto nonExistingDir = existing_dir + kSuffix;

    ASSERT_TRUE(fs_.CopyDirectory(existing_dir, nonExistingDir));

    ASSERT_TRUE(fs_.RemoveDirectory(nonExistingDir));
}

TEST_F(FilesystemTestFixture, GetDirectorySize_InvalidArgument)
{
    ASSERT_FALSE(fs_.GetDirectorySize("/non/existing/dir"));
    ASSERT_FALSE(fs_.GetDirectorySize(""));
    ASSERT_FALSE(fs_.GetDirectorySize("xyz123"));
}

TEST_F(FilesystemTestFixture, GetDirectorySize)
{
    uint64_t dirSize = fs_.GetDirectorySize(kTestOptPath).Value();
    ASSERT_THAT(dirSize, Eq(kTestOptPathSize));
}

TEST_F(FilesystemTestFixture, GetFiles_Positive)
{
    auto result = fs_.GetFiles(kTestDataPath);

    ASSERT_THAT(result.size(), 4);
    ASSERT_THAT(std::find(result.begin(), result.end(), kTestInstallSwPackagePath), Ne(result.end()));
    ASSERT_THAT(std::find(result.begin(), result.end(), kTestUninstallSwPackagePath), Ne(result.end()));
    ASSERT_THAT(std::find(result.begin(), result.end(), kTestUpdateSwPackagePath), Ne(result.end()));
}

TEST_F(FilesystemTestFixture, GetSubdirectories_Positive)
{
    ara::core::String source = "/usr/var/apdtest";
    ara::core::String target = kTestWorkingDir;

    auto result = fs_.GetSubdirectories(source);

    ASSERT_THAT(result.size(), 1);
    ASSERT_THAT(result[0], target);
}

TEST_F(FilesystemTestFixture, GetFilesRecursive)
{
    auto result = fs_.GetFilesRecursive(kTestOptPath + "/swcls");

    ASSERT_THAT(result.size(), 9);

    int manifestFilesCount = 0;

    for (size_t i = 0; i < result.size(); i++) {
        if (result[i].find("MANIFEST") != ara::core::String::npos)
            manifestFilesCount++;
    }

    ASSERT_THAT(manifestFilesCount, 3);
}

}  //  namespace pkgmgr
}  //  namespace ucm
}  //  namespace ara
