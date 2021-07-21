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
#include "test_helper.h"
#include <limits>

#include "package_manager.h"
#include "ara/ucm/pkgmgr/portinterfaces/PackageManagement.h"

using ::testing::Eq;
using ::testing::Ne;
using ::testing::_;
using ::testing::Return;
using ara::ucm::pkgmgr::TransferDataReturnType;
using ara::ucm::pkgmgr::TransferExitReturnType;
using ara::ucm::pkgmgr::TransferStartReturnType;
using ara::ucm::pkgmgr::TransferStartSuccessType;
using ara::ucm::pkgmgr::TransferIdType;
using ara::ucm::pkgmgr::ByteVectorType;

namespace ara
{
namespace ucm
{
namespace pkgmgr
{

class TransferDataSequenceFixture : public ::testing::Test
{
public:
    TransferDataSequenceFixture()
        : pm_(fs_, builder_, "")
    {}

    TransferIdType SetupTransfer(const std::uint32_t size)
    {
        ExpectMaximalDiskSpace();
        TransferStartReturnType result = pm_.TransferStart(size);
        EXPECT_EQ(result.TransferStartSuccess, (TransferStartSuccessType::kSuccess));
        return result.TransferId;
    }

    const ByteVectorType CreateDataBlockWithSize(const std::uint32_t size)
    {
        ByteVectorType block;
        for (uint32_t i = 0; i < size; i++) {
            block.push_back(0xFF);
        }
        return block;
    }

    std::uint8_t TransmitBlocks(int amountOfBlocksToTransmit, TransferIdType id, int blockCounter = 1)
    {
        for (; blockCounter < amountOfBlocksToTransmit; blockCounter++) {
            pm_.TransferData(id, CreateDataBlockWithSize(1), blockCounter);
        }
        return blockCounter;
    }

    void ExpectMaximalDiskSpace()
    {
        EXPECT_CALL(fs_, GetFreeDiskSpace(_)).WillRepeatedly(Return(std::numeric_limits<uint64_t>::max()));
    }

    FilesystemMock fs_;
    ApplicationListBuilderMock builder_;
    PackageManager pm_;
};

TEST_F(TransferDataSequenceFixture, CanGetUniqueTransferId)
{
    std::set<TransferIdType> startedSessions;
    constexpr int runs = 100;
    for (int i = 0; i < runs; i++) {
        TransferIdType newSession = SetupTransfer(1);

        ASSERT_THAT(startedSessions.find(newSession), Eq(startedSessions.end()));

        startedSessions.insert(newSession);
    }
}

TEST_F(TransferDataSequenceFixture, CanStartTransfer)
{
    ExpectMaximalDiskSpace();
    TransferStartReturnType result = pm_.TransferStart(1);

    ASSERT_THAT(result.TransferStartSuccess, Eq(TransferStartSuccessType::kSuccess));
}

TEST_F(TransferDataSequenceFixture, CanStartTransferConcurrently)
{
    ExpectMaximalDiskSpace();
    TransferStartReturnType result1 = pm_.TransferStart(1);
    TransferStartReturnType result2 = pm_.TransferStart(1);

    ASSERT_THAT(result1.TransferStartSuccess, Eq(TransferStartSuccessType::kSuccess));
    ASSERT_THAT(result2.TransferStartSuccess, Eq(TransferStartSuccessType::kSuccess));
}

TEST_F(TransferDataSequenceFixture, DoesRejectTransferIfInsufficientSize)
{
    EXPECT_CALL(fs_, GetFreeDiskSpace(_)).WillOnce(Return(500));

    TransferStartReturnType result = pm_.TransferStart(std::numeric_limits<std::uint32_t>::max());

    ASSERT_THAT(result.TransferStartSuccess, Eq(TransferStartSuccessType::kInsufficientMemory));
}

TEST_F(TransferDataSequenceFixture, DoesReturnInsufficientData_NoDataTransferred)
{
    TransferIdType transferId = SetupTransfer(123);

    TransferExitReturnType result = pm_.TransferExit(transferId);

    ASSERT_THAT(result, Eq(TransferExitReturnType::kInsufficientData));
}

TEST_F(TransferDataSequenceFixture, DoesTransferDataForSingleBlock)
{
    TransferIdType transferId = SetupTransfer(1);

    TransferDataReturnType transferDataResult = pm_.TransferData(transferId, {0xFF}, 1);

    ASSERT_THAT(transferDataResult, Eq(TransferDataReturnType::kSuccess));
}

TEST_F(TransferDataSequenceFixture, DoesTransferDataConcurrently)
{
    TransferIdType session1 = SetupTransfer(1);
    TransferIdType session2 = SetupTransfer(1);

    TransferDataReturnType transferDataResultSession1 = pm_.TransferData(session1, {0xFF}, 1);
    TransferDataReturnType transferDataResultSession2 = pm_.TransferData(session2, {0xF0}, 1);

    ASSERT_THAT(transferDataResultSession1, Eq(TransferDataReturnType::kSuccess));
    ASSERT_THAT(transferDataResultSession2, Eq(TransferDataReturnType::kSuccess));
}

TEST_F(TransferDataSequenceFixture, DoesReturnInsufficientData_LessDataThanAdvertised)
{
    uint32_t dataSizeAdvertisedInBytes = 10000;
    uint32_t dataSizeActuallyTransmittedInBytes = 1;
    TransferIdType transferId = SetupTransfer(dataSizeAdvertisedInBytes);
    pm_.TransferData(transferId, CreateDataBlockWithSize(dataSizeActuallyTransmittedInBytes), 1);

    TransferExitReturnType result = pm_.TransferExit(transferId);

    ASSERT_THAT(result, Eq(TransferExitReturnType::kInsufficientData));
}

TEST_F(TransferDataSequenceFixture, DoesReturnInsufficientData_NoDataSent)
{
    uint32_t dataSizeAdvertisedInBytes = 10000;
    TransferIdType transferId = SetupTransfer(dataSizeAdvertisedInBytes);

    TransferExitReturnType result = pm_.TransferExit(transferId);

    ASSERT_THAT(result, Eq(TransferExitReturnType::kInsufficientData));
}

TEST_F(TransferDataSequenceFixture, DoesDetectMoreDataThanAdvertised)
{
    uint32_t dataSizeAdvertisedInBytes = 1;
    uint32_t dataSizeActuallyTransmittedInBytes = 10;
    TransferIdType transferId = SetupTransfer(dataSizeAdvertisedInBytes);

    TransferDataReturnType transferDataResult
        = pm_.TransferData(transferId, CreateDataBlockWithSize(dataSizeActuallyTransmittedInBytes), 1);

    ASSERT_THAT(transferDataResult, Eq(TransferDataReturnType::kIncorrectSize));
}

TEST_F(TransferDataSequenceFixture, DoesDetectRetransmittedBlock)
{
    TransferIdType transferId = SetupTransfer(100);
    const std::uint8_t blockCounter = TransmitBlocks(1, transferId);

    TransferDataReturnType transferDataResult = pm_.TransferData(transferId, CreateDataBlockWithSize(1), blockCounter);

    ASSERT_THAT(transferDataResult, Eq(TransferDataReturnType::kIncorrectBlock));
}

TEST_F(TransferDataSequenceFixture, DoesRejectBlockCounterZero)
{
    TransferIdType transferId = SetupTransfer(100);

    TransferDataReturnType transferDataResult = pm_.TransferData(transferId, CreateDataBlockWithSize(1), 0);

    ASSERT_THAT(transferDataResult, Eq(TransferDataReturnType::kIncorrectBlock));
}

TEST_F(TransferDataSequenceFixture, DoesDetectWrongBlockOrdering)
{
    TransferIdType transferId = SetupTransfer(100);
    std::uint8_t blockCounter = TransmitBlocks(3, transferId);
    blockCounter += 2;

    TransferDataReturnType transferDataResult = pm_.TransferData(transferId, CreateDataBlockWithSize(1), blockCounter);

    ASSERT_THAT(transferDataResult, Eq(TransferDataReturnType::kIncorrectBlock));
}

TEST_F(TransferDataSequenceFixture, DoesTransferVariableSizeBlocks)
{
    TransferIdType transferId = SetupTransfer(3);
    TransferDataReturnType transferDataResult = pm_.TransferData(transferId, {0x12, 0x34}, 1);
    EXPECT_THAT(transferDataResult, Eq(TransferDataReturnType::kSuccess));

    transferDataResult = pm_.TransferData(transferId, {0x56}, 2);

    ASSERT_THAT(transferDataResult, Eq(TransferDataReturnType::kSuccess));
}

TEST_F(TransferDataSequenceFixture, DoesDetectInvalidId_TransferData)
{
    TransferIdType invalidTransferId = 123;

    TransferDataReturnType result = pm_.TransferData(invalidTransferId, CreateDataBlockWithSize(1), 1);

    ASSERT_THAT(result, Eq(TransferDataReturnType::kInvalidTransferId));
}

TEST_F(TransferDataSequenceFixture, DoesDetectInvalidId_TransferExit)
{
    TransferIdType invalidTransferId = 123;

    TransferExitReturnType result = pm_.TransferExit(invalidTransferId);

    ASSERT_THAT(result, Eq(TransferExitReturnType::kInvalidTransferId));
}

TEST_F(TransferDataSequenceFixture, DoesDetectNotPermittetOperation)
{
    TransferIdType transferId = SetupTransfer(3);

    TransferExitReturnType result = pm_.TransferExit(transferId);

    ASSERT_THAT(result, Eq(TransferExitReturnType::kOperationNotPermitted));
}

}  // namespace pkgmgr
}  // namespace ucm
}  // namespace ara
