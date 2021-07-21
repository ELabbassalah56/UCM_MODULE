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

namespace ara
{
namespace ucm
{
namespace pkgmgr
{

using strvec = ara::core::Vector<ara::core::String>;
using testing::UnorderedElementsAreArray;

class ProcessesListManagerTestFixture : public ::testing::Test
{

public:
    const ara::core::String filename = kTestOptPath + "/PROCESSES.json";
    const ara::core::String kStringKeyName_ = "processes";
    Filesystem fs_;
    std::unique_ptr<ProcessesListManager> manager_;
    std::unique_ptr<SWCLManager> swclManager_;
    strvec appsPrefix_;

    void SetUp()
    {
        manager_ = std::make_unique<ProcessesListManager>(kTestOptPath);
        swclManager_ = std::make_unique<FileSystemSWCLManager>(kSwclDir);

        // write to file
        std::ofstream file;
        file.open(filename.c_str());
        file << testdata::sample_processes_list;
        file.close();

        ApplicationListBuilder builder_(fs_, kTestOptPath);
        ara::core::Vector<Application> apps = builder_.GetApplications();

        for (const auto& app : apps) {
            appsPrefix_.push_back(app.GetApplicationPrefix());
        }

        CreateSWCLsFolderStructure();
        appsPrefix_.push_back(kSwcl_A_App1Proc1Dir);
        appsPrefix_.push_back(kSwcl_A_App1Proc2Dir);
        appsPrefix_.push_back(kSwcl_A_App2Proc1Dir);
        appsPrefix_.push_back(kSwcl_B_App1Proc1Dir);
        appsPrefix_.push_back(kSwcl_C_App1Proc1Dir);
    }

    void TearDown()
    {
        std::remove(filename.c_str());
        RemoveSWCLsFolderStructure();
    }
};

TEST_F(ProcessesListManagerTestFixture, UpdateProcessesList)
{
    manager_->UpdateProcessesList(swclManager_->GetSWCLsForActivation());

    auto kvs = ara::per::OpenKeyValueStorage(filename);
    auto db = std::move(kvs).Value();
    strvec result;
    db->GetValue(kStringKeyName_, result);

    EXPECT_THAT(appsPrefix_, UnorderedElementsAreArray(result));
}

}  // namespace pkgmgr
}  // namespace ucm
}  // namespace ara
