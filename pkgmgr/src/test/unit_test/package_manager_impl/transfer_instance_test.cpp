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

#include "transfer/transfer_instance.h"
#include "transfer/receive_types.h"

namespace ara
{
namespace ucm
{
namespace pkgmgr
{

using ::testing::Eq;
using ::testing::StrEq;

class TransferInstanceTestFixture : public ::testing::Test
{
public:
    /// TODO: get the unit test directory from some global definition
    TransferInstanceTestFixture()
        : ti(id, "/usr/var/apdtest/ucm")
    {}

    TransferInstance ti;
    const TransferIdType id = 335;

    void SetUp()
    {}

    void TearDown()
    {}
};

// TransferExit unit-test
TEST_F(TransferInstanceTestFixture, TransferExit_Success)
{
    TransferStartReturnType tsReturn;
    TransferDataReturnType tdReturn;
    TransferExitReturnType teReturn;

    ara::core::String strData = "FT-UCM";
    ByteVectorType byteData(strData.begin(), strData.end());

    // Activate TransferStart (change state) (datasize should be 6)
    tsReturn = ti.TransferStart(6);
    EXPECT_THAT(tsReturn.TransferStartSuccess, Eq(TransferStartSuccessType::kSuccess));

    // Activate TransferData (stream data correctly)
    tdReturn = ti.TransferData(byteData, 1);  // data, blockcounter
    EXPECT_THAT(tdReturn, Eq(TransferDataReturnType::kSuccess));

    teReturn = ti.TransferExit();
    ASSERT_THAT(teReturn, Eq(TransferExitReturnType::kSuccess));
}

TEST_F(TransferInstanceTestFixture, TransferExit_WrongOperation)
{
    TransferExitReturnType teReturn;

    teReturn = ti.TransferExit();
    ASSERT_THAT(teReturn, Eq(TransferExitReturnType::kOperationNotPermitted));
}

TEST_F(TransferInstanceTestFixture, TransferExit_LackOfData)
{
    TransferStartReturnType tsReturn;
    TransferDataReturnType tdReturn;
    TransferExitReturnType teReturn;

    ara::core::String strData = "FT-UCM";
    ByteVectorType byteData(strData.begin(), strData.end());

    // Activate TransferStart (change state) (datasize should be 6)
    tsReturn = ti.TransferStart(10);
    EXPECT_THAT(tsReturn.TransferStartSuccess, Eq(TransferStartSuccessType::kSuccess));

    // Activate TransferData (stream data correctly)
    tdReturn = ti.TransferData(byteData, 1);  // data, blockcounter
    EXPECT_THAT(tdReturn, Eq(TransferDataReturnType::kSuccess));

    teReturn = ti.TransferExit();
    ASSERT_THAT(teReturn, Eq(TransferExitReturnType::kInsufficientData));
}

// TransferData unit-test
TEST_F(TransferInstanceTestFixture, TransferData_Success)
{
    TransferStartReturnType tsReturn;
    TransferDataReturnType tdReturn;

    ara::core::String strData1 = "ADAPTIVE ";  // 9 bytes
    ByteVectorType byteData1(strData1.begin(), strData1.end());
    ara::core::String strData2 = "AUTOSAR ";  // 8 bytes
    ByteVectorType byteData2(strData2.begin(), strData2.end());
    ara::core::String strData3 = "FT-UCM";  // 6 bytes
    ByteVectorType byteData3(strData3.begin(), strData3.end());

    // Activate TransferStart (change state) (datasize should be 23)
    tsReturn = ti.TransferStart(23);
    EXPECT_THAT(tsReturn.TransferStartSuccess, Eq(TransferStartSuccessType::kSuccess));

    // Activate TransferData 1of3
    tdReturn = ti.TransferData(byteData1, 1);  // data, blockcounter
    EXPECT_THAT(tdReturn, Eq(TransferDataReturnType::kSuccess));

    // Activate TransferData 2of3
    tdReturn = ti.TransferData(byteData2, 2);  // data, blockcounter
    EXPECT_THAT(tdReturn, Eq(TransferDataReturnType::kSuccess));

    // Activate TransferData 3of3
    tdReturn = ti.TransferData(byteData3, 3);  // data, blockcounter
    ASSERT_THAT(tdReturn, Eq(TransferDataReturnType::kSuccess));
}

TEST_F(TransferInstanceTestFixture, TransferData_WrongOperation)
{
    TransferDataReturnType tdReturn;

    ara::core::String strData1 = "ADAPTIVE ";  // 9 bytes
    ByteVectorType byteData1(strData1.begin(), strData1.end());

    // Activate TransferData 1of3
    tdReturn = ti.TransferData(byteData1, 1);  // data, blockcounter
    ASSERT_THAT(tdReturn, Eq(TransferDataReturnType::kOperationNotPermitted));
}

TEST_F(TransferInstanceTestFixture, TransferData_ExceedDataSize)
{
    TransferStartReturnType tsReturn;
    TransferDataReturnType tdReturn;

    ara::core::String strData1 = "ADAPTIVE ";  // 9 bytes
    ByteVectorType byteData1(strData1.begin(), strData1.end());
    ara::core::String strData2 = "AUTOSAR ";  // 8 bytes
    ByteVectorType byteData2(strData2.begin(), strData2.end());
    ara::core::String strData3 = "FT-UCM";  // 6 bytes
    ByteVectorType byteData3(strData3.begin(), strData3.end());

    // Activate TransferStart (change state) (datasize should be 23)
    tsReturn = ti.TransferStart(20);
    EXPECT_THAT(tsReturn.TransferStartSuccess, Eq(TransferStartSuccessType::kSuccess));

    // Activate TransferData 1of3
    tdReturn = ti.TransferData(byteData1, 1);  // data, blockcounter
    EXPECT_THAT(tdReturn, Eq(TransferDataReturnType::kSuccess));

    // Activate TransferData 2of3
    tdReturn = ti.TransferData(byteData2, 2);  // data, blockcounter
    EXPECT_THAT(tdReturn, Eq(TransferDataReturnType::kSuccess));

    // Activate TransferData 3of3
    tdReturn = ti.TransferData(byteData3, 3);  // data, blockcounter
    ASSERT_THAT(tdReturn, Eq(TransferDataReturnType::kIncorrectSize));
}

TEST_F(TransferInstanceTestFixture, TransferData_WrongDataBlock)
{
    TransferStartReturnType tsReturn;
    TransferDataReturnType tdReturn;

    ara::core::String strData1 = "ADAPTIVE ";  // 9 bytes
    ByteVectorType byteData1(strData1.begin(), strData1.end());
    ara::core::String strData2 = "AUTOSAR ";  // 8 bytes
    ByteVectorType byteData2(strData2.begin(), strData2.end());
    ara::core::String strData3 = "FT-UCM";  // 6 bytes
    ByteVectorType byteData3(strData3.begin(), strData3.end());

    // Activate TransferStart (change state) (datasize should be 23)
    tsReturn = ti.TransferStart(23);
    EXPECT_THAT(tsReturn.TransferStartSuccess, Eq(TransferStartSuccessType::kSuccess));

    // Activate TransferData 1of3
    tdReturn = ti.TransferData(byteData1, 1);  // data, blockcounter
    EXPECT_THAT(tdReturn, Eq(TransferDataReturnType::kSuccess));

    // Activate TransferData 3of3
    tdReturn = ti.TransferData(byteData3, 3);  // data, blockcounter
    ASSERT_THAT(tdReturn, Eq(TransferDataReturnType::kIncorrectBlock));
}

// TransferStart unit-test
TEST_F(TransferInstanceTestFixture, TransferStart_Success)
{
    TransferStartReturnType tsReturn;

    tsReturn = ti.TransferStart(100);
    ASSERT_THAT(tsReturn.TransferStartSuccess, Eq(TransferStartSuccessType::kSuccess));
}

TEST_F(TransferInstanceTestFixture, TransferStart_ExceedFreeLocalMemory)
{
    TransferStartReturnType tsReturn;

    tsReturn = ti.TransferStart(UINT32_MAX);
    ASSERT_THAT(tsReturn.TransferStartSuccess, Eq(TransferStartSuccessType::kInsufficientMemory));
}

// GetPackage unit-test
TEST_F(TransferInstanceTestFixture, GetPackagePath)
{
    TransferStartReturnType tsReturn;
    TransferDataReturnType tdReturn;
    TransferExitReturnType teReturn;
    ara::core::String gppReturn;

    ara::core::String strData = "FT-UCM";
    ByteVectorType byteData(strData.begin(), strData.end());

    // Activate TransferStart (change state) (datasize should be 6)
    tsReturn = ti.TransferStart(6);
    EXPECT_THAT(tsReturn.TransferStartSuccess, Eq(TransferStartSuccessType::kSuccess));

    // Activate TransferData (stream data correctly)
    tdReturn = ti.TransferData(byteData, 1);  // data, blockcounter
    EXPECT_THAT(tdReturn, Eq(TransferDataReturnType::kSuccess));

    teReturn = ti.TransferExit();
    EXPECT_THAT(teReturn, Eq(TransferExitReturnType::kSuccess));

    gppReturn = ti.GetPackagePath();

    ASSERT_EQ(gppReturn, "/usr/var/apdtest/ucm/335.zip");
}

}  // namespace pkgmgr
}  // namespace ucm
}  // namespace ara
