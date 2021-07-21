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

#include "parsing/application.h"
#include "manifest_samples.h"

#include "boost/property_tree/json_parser.hpp"
#include "boost/property_tree/ptree.hpp"
#include <cstdio>
#include <fstream>
#include <cstdint>

namespace ara
{
namespace ucm
{
namespace pkgmgr
{

using ::testing::Eq;
using ::testing::StrEq;

class ApplicationTestFixture : public ::testing::Test
{

public:
    const ara::core::String filename = "./sample_application_manifest.json";

    void SetUp()
    {
        // write to file
        std::ofstream file;
        file.open(filename.c_str());
        file << testdata::sample_application_manifest;
        file.close();
    }

    void TearDown()
    {
        std::remove(filename.c_str());
    }
};

TEST_F(ApplicationTestFixture, GetApplicationPrefix)
{
    Application app("/my/prefix", "_", "_", filename);

    ASSERT_THAT(app.GetApplicationPrefix(), Eq("/my/prefix"));
}

TEST_F(ApplicationTestFixture, GetExecutable)
{
    Application app("_", "myexecutable", "_", filename);

    ASSERT_THAT(app.GetExecutable(), Eq("myexecutable"));
}

TEST_F(ApplicationTestFixture, GetApplicationName)
{
    Application app("_", "_", "myapp", filename);

    ASSERT_THAT(app.GetApplicationName(), Eq("myapp"));
}

TEST_F(ApplicationTestFixture, GetDependencies)
{
    Application app("_", "_", "_", filename);

    ara::core::Vector<ara::core::String> dependencies = app.GetDependencies();

    ASSERT_THAT(dependencies.size(), Eq(2));
    ASSERT_THAT(dependencies[0], Eq("Calculator"));
    ASSERT_THAT(dependencies[1], Eq("DiagnosticManager"));
}

TEST_F(ApplicationTestFixture, GetVersion)
{
    Application app("_", "_", "_", filename, Version("3.141"));

    ASSERT_TRUE(app.GetApplicationVersion() == Version("3.141"));
}

TEST_F(ApplicationTestFixture, CompareVersion)
{
    Application app("_", "_", "_", filename, Version("3.141"));
    Version vMajorLowerMinorLower = Version("2.140");
    Version vMajorLowerMinorEqual = Version("2.141");
    Version vMajorLowerMinorGreater = Version("2.142");
    Version vMajorEqualMinorLower = Version("3.140");
    Version vMajorEqualMinorEqual = Version("3.141");
    Version vMajorEqualMinorGreater = Version("3.142");
    Version vMajorGreaterMinorLower = Version("4.140");
    Version vMajorGreaterMinorEqual = Version("4.141");
    Version vMajorGreaterMinorGreater = Version("4.142");

    ASSERT_TRUE(app.GetApplicationVersion() > vMajorLowerMinorLower);
    ASSERT_TRUE(app.GetApplicationVersion() > vMajorLowerMinorEqual);
    ASSERT_TRUE(app.GetApplicationVersion() > vMajorLowerMinorGreater);
    ASSERT_TRUE(app.GetApplicationVersion() > vMajorEqualMinorLower);
    ASSERT_TRUE(app.GetApplicationVersion() == vMajorEqualMinorEqual);
    ASSERT_TRUE(app.GetApplicationVersion() < vMajorEqualMinorGreater);
    ASSERT_TRUE(app.GetApplicationVersion() < vMajorGreaterMinorLower);
    ASSERT_TRUE(app.GetApplicationVersion() < vMajorGreaterMinorEqual);
    ASSERT_TRUE(app.GetApplicationVersion() < vMajorGreaterMinorGreater);
}

}  // namespace pkgmgr
}  // namespace ucm
}  // namespace ara
