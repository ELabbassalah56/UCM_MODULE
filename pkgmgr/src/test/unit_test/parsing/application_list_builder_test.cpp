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

#include <iostream>
#include <stdio.h>
#include <fstream>

#include "filesystem.h"
#include "parsing/application_list_builder.h"
#include "parsing/application.h"
#include "test_helper.h"

namespace ara
{
namespace ucm
{
namespace pkgmgr
{

using ::testing::Eq;
using ::testing::Ne;

class ApplicationListBuilderTestFixture : public ::testing::Test
{
public:
    ApplicationListBuilderTestFixture()
        : builder_(fs_, kTestOptPath)
    {}

    const Application* FindApp(const ara::core::Vector<Application>& apps, ara::core::String appname)
    {

        for (auto& app : apps) {
            if (appname.compare(app.GetApplicationName()) == 0) {
                return &app;
            }
        }
        return nullptr;
    }

    Filesystem fs_;
    ApplicationListBuilder builder_;

    using strvec = ara::core::Vector<ara::core::String>;
    strvec PrefixRef, ExecRef, AppNameRef, ManPathRef, VersionRef;

    void SetUp()
    {
        PrefixRef.emplace_back(kTestOptPath + "/DiagnosticExample");
        PrefixRef.emplace_back(kTestOptPath + "/DiagnosticManager");
        ExecRef.emplace_back(kTestOptPath + "/DiagnosticExample/bin/Calculator");
        ExecRef.emplace_back(kTestOptPath + "/DiagnosticManager/bin/DiagnosticManager");
        AppNameRef.emplace_back("Calculator");
        AppNameRef.emplace_back("DiagnosticManager");
        ManPathRef.emplace_back(kTestOptPath + "/DiagnosticExample/etc/MANIFEST.json");
        ManPathRef.emplace_back(kTestOptPath + "/DiagnosticManager/etc/MANIFEST.json");
        VersionRef.emplace_back("1.0");
        VersionRef.emplace_back("1.0");
    }
};

TEST_F(ApplicationListBuilderTestFixture, TestGetApplicationCount)
{
    ASSERT_THAT(builder_.GetApplicationCount(), Eq(2));
}

TEST_F(ApplicationListBuilderTestFixture, TestGetApplications)
{
    const ara::core::Vector<Application>& apps = builder_.GetApplications();
    EXPECT_THAT(builder_.GetApplicationCount(), Eq(2));

    ara::core::String dm = "DiagnosticManager";
    ara::core::String calc = "Calculator";

    const Application* dm_app = FindApp(apps, dm);
    ASSERT_THAT(dm_app, Ne(nullptr));

    const Application* calc_app = FindApp(apps, calc);
    ASSERT_THAT(calc_app, Ne(nullptr));
}

TEST_F(ApplicationListBuilderTestFixture, TestRefreshList)
{
    builder_.RefreshList();

    auto appvec = builder_.GetApplications();

    strvec AppNames;
    strvec ManPaths;
    strvec Execs;
    strvec Prefixes;
    strvec Versions;

    for (auto& app : appvec) {
        AppNames.push_back(app.GetApplicationName());
        Prefixes.push_back(app.GetApplicationPrefix());
        Execs.push_back(app.GetExecutable());
        ManPaths.push_back(app.GetManifestPath());
        Versions.push_back(app.GetApplicationVersion().ToString());
    }

    using testing::UnorderedElementsAreArray;

    EXPECT_THAT(PrefixRef, UnorderedElementsAreArray(Prefixes));
    EXPECT_THAT(ExecRef, UnorderedElementsAreArray(Execs));
    EXPECT_THAT(AppNameRef, UnorderedElementsAreArray(AppNames));
    EXPECT_THAT(ManPathRef, UnorderedElementsAreArray(ManPaths));
    EXPECT_THAT(VersionRef, UnorderedElementsAreArray(Versions));
}

}  // namespace pkgmgr
}  // namespace ucm
}  // namespace ara
