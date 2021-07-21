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

#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "ara/core/string.h"

#include "test_helper.h"
#include "transfer/software_package_data.h"

using ::testing::Eq;
using ::testing::Ne;
using ::testing::Ge;
using ::testing::NiceMock;
using ::testing::Return;
using ::testing::ReturnRef;
using ::testing::Invoke;
using ::testing::_;

namespace ara
{
namespace ucm
{
namespace pkgmgr
{

class SoftwarePackageDataFixture : public ::testing::Test
{
public:
    SoftwarePackageDataFixture()
    {}

    void SetUp()
    {
        data_ = std::make_unique<NiceMock<SoftwarePackageDataMock>>();
    }

    std::unique_ptr<NiceMock<SoftwarePackageDataMock>>& GetPackageData()
    {
        return data_;
    }

private:
    std::unique_ptr<NiceMock<SoftwarePackageDataMock>> data_;
};

TEST_F(SoftwarePackageDataFixture, TransferStartCallProducesLogMessage)
{
    // SETUP
    std::uint32_t size = 100;

    std::unique_ptr<TransferInstanceMock> mockTransfer = std::make_unique<TransferInstanceMock>();
    std::unique_ptr<Streamable> mockStreamamble(static_cast<Streamable*>(mockTransfer.get()));

    EXPECT_CALL(*mockTransfer, TransferStart(_))
        .Times(1)
        .WillOnce(Return(TransferStartReturnType({1, TransferStartSuccessType::kSuccess})));
    EXPECT_CALL(*GetPackageData(), GetStreamable()).WillRepeatedly(ReturnRef(mockStreamamble));
    ON_CALL(*GetPackageData(), TransferStart(_))
        .WillByDefault(Invoke(GetPackageData().get(), &SoftwarePackageDataMock::CallTransferStart));

    // TEST
    GetPackageData()->TransferStart(size);
    auto logs = GetPackageData()->GetLogger().GetLogs();

    // ASSERTS
    ASSERT_THAT(logs.size(), Ge(2));

    bool bFound = false;

    for (size_t i = 0; i < logs.size(); i++) {
        auto transferStartSearchResult = logs[i].Message.find("TransferStart");

        if (transferStartSearchResult != ara::core::String::npos) {
            bFound = true;

            ASSERT_THAT(logs[i].LogLevel, LogLevelType::kVerbose);
            ASSERT_THAT(logs[0].Message.empty(), false);

            break;
        }
    }

    ASSERT_THAT(bFound, true);

    mockStreamamble.release();  // mockTransfer will care about the pointer
}

}  // namespace pkgmgr
}  // namespace ucm
}  // namespace ara
