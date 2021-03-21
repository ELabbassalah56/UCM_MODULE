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
#include "test/unit_test/test_helper.h"
#include "package_manager_state/package_manager_idle_state.h"
#include "package_manager/package_manager.h"
#include "transfer/receive_types.h"
#include "transfer/software_package_data.h"
#include "memory"
#include "ara/ucm/pkgmgr/impl_type_logleveltype.h"
#include "synchronized_storage.h"
#include "boost/property_tree/ptree.hpp"
#include "boost/property_tree/json_parser.hpp"
#include "ara/core/string.h"
#include "package_management_application.h"

using testing::Return;
using testing::ByMove;
using testing::NiceMock;
using testing::ReturnRef;

namespace ara
{
namespace ucm
{
namespace pkgmgr
{

class SynchronizedStorageMock;

class PackageManagerImplMock : public PackageManagerImpl
{
public:
  // create constractor intialized dependencies from pkgmngrImpl
    PackageManagerImplMock()
        : PackageManagerImpl(std::make_unique<SoftwarePackageExtractorMock>(),
              std::make_unique<SoftwarePackageParserStub>(testdata::CreateSoftwarePackageManifestObject("Update")),
              std::make_unique<SWCLManagerStub>(),
              std::make_unique<ProcessesListManager>(kTestOptPath),
              std::make_unique<StreamableSoftwarePackageFactoryStub>(),
              std::make_unique<SynchronizedStorage<TransferIdType, StreamableSoftwarePackage>>(),
              std::string("/tmp"))
    {}

    MOCK_METHOD2(ParseSoftwarePackage,
        SoftwarePackage(const ara::core::String& extractionDir, const SoftwarePackageManifest& manifest));
    MOCK_METHOD3(TransferData,
        TransferDataFuture(const TransferIdType& id, const ByteVectorType& data, const std::uint32_t& blockCounter));
    MOCK_METHOD1(TransferExit, TransferExitFuture(const TransferIdType& id));
    MOCK_METHOD1(TransferStart, TransferStartFuture(const std::uint32_t& size));
    MOCK_METHOD1(ProcessSoftwarePackage, Result<void, ErrorCode>(const core::String& path));
    MOCK_CONST_METHOD0(GetSwPackages, GetSwPackagesOutput());
    MOCK_CONST_METHOD1(GetLog, ara::core::Future<PackageManagement::GetLogOutput>(const TransferIdType id));
    MOCK_CONST_METHOD0(GetSwClusterInfo, SwClusterInfoVectorType());
    MOCK_METHOD0(RetrieveState, std::unique_ptr<PackageManagerState>());
    MOCK_METHOD1(DoProcessSwPackage, Result<void, ErrorCode>(const TransferIdType& id));
    MOCK_METHOD1(DeleteTransfer, ara::core::Future<void>(const TransferIdType& id));
    MOCK_METHOD1(Activate, ara::core::Future<void>(const ActivateOptionType& option));  // b3bast here 3abbas
    MOCK_CONST_METHOD1(Cancel,ara::core::Future<void>(const TransferIdType& id)); // b3bast here 3abbas
    MOCK_METHOD1(ProcessSwPackage,Result<void, ErrorCode>(const TransferIdType& id)); // b3bast here 3abbas

};

using NicePackageManagerImplMock = NiceMock<PackageManagerImplMock>;

class PackageManagerFixture : public ::testing::Test
{

public:
    PackageManagerFixture()
        : pmIntMock_(std::make_unique<NicePackageManagerImplMock>())
    {}

protected:
    /// Note that you need to set any mock expectations
    /// before accessing the package manager via this method
    std::unique_ptr<skeleton::PackageManagementSkeleton> ServiceInstance()
    {
        EXPECT_CALL(*pmIntMock_, RetrieveState())
            .WillOnce(Return(ByMove(std::make_unique<PackageManagerIdleState>())));
        return std::make_unique<PackageManager>(std::move(pmIntMock_), 828);
    }

    std::unique_ptr<NicePackageManagerImplMock> pmIntMock_;

    TransferIdType id{42};
    std::uint32_t size{1024};
    
};

TEST_F(PackageManagerFixture, TransferStart_Success)
{
    TransferStartFuture::PromiseType promise;
    auto future = promise.get_future();
    promise.set_value({id});

    EXPECT_CALL(*pmIntMock_, TransferStart(size)).WillOnce(Return(ByMove(std::move(future))));

    auto service = ServiceInstance();
    auto result = service->TransferStart(size).GetResult();

    ASSERT_TRUE(result);
    ASSERT_EQ(result.Value().id, id);
}

TEST_F(PackageManagerFixture, TransferStart_InsufficientMemory)
{
    TransferStartFuture::PromiseType promise;
    auto future = promise.get_future();
    promise.SetError(UCMErrorDomainErrc::kInsufficientMemory);

    EXPECT_CALL(*pmIntMock_, TransferStart(size)).WillOnce(Return(ByMove(std::move(future))));

    auto service = ServiceInstance();
    auto result = service->TransferStart(size).GetResult();

    ASSERT_FALSE(result);
    ASSERT_EQ(result.Error(), UCMErrorDomainErrc::kInsufficientMemory);
}

TEST_F(PackageManagerFixture, TransferData_Success)
{
    TransferDataFuture::PromiseType promise;
    auto future = promise.get_future();
    promise.set_value();

    ByteVectorType data{0xFF, 0xFF};
    EXPECT_CALL(*pmIntMock_, TransferData(size, data, 1)).WillOnce(Return(ByMove(std::move(future))));

    auto service = ServiceInstance();
    auto result = service->TransferData(size, data, 1).GetResult();

    ASSERT_TRUE(result);
}

TEST_F(PackageManagerFixture, TransferData_IncorrectBlock)
{
    TransferDataFuture::PromiseType promise;
    auto future = promise.get_future();
    promise.SetError(UCMErrorDomainErrc::kIncorrectBlock);

    ByteVectorType data{0xFF, 0xFF};
    EXPECT_CALL(*pmIntMock_, TransferData(size, data, 1024)).WillOnce(Return(ByMove(std::move(future))));

    auto service = ServiceInstance();
    auto result = service->TransferData(size, data, 1024).GetResult();

    ASSERT_FALSE(result);
    ASSERT_EQ(result.Error(), UCMErrorDomainErrc::kIncorrectBlock);
}

TEST_F(PackageManagerFixture, TransferExit_Success)
{
    TransferExitFuture::PromiseType promise;
    auto future = promise.get_future();
    promise.set_value();

    EXPECT_CALL(*pmIntMock_, TransferExit(id)).WillOnce(Return(ByMove(std::move(future))));

    auto service = ServiceInstance();
    auto result = service->TransferExit(id).GetResult();

    ASSERT_TRUE(result);
}

TEST_F(PackageManagerFixture, TransferExit_InsufficientData)
{
    TransferExitFuture::PromiseType promise;
    auto future = promise.get_future();
    promise.SetError(UCMErrorDomainErrc::kInsufficientData);

    EXPECT_CALL(*pmIntMock_, TransferExit(id)).WillOnce(Return(ByMove(std::move(future))));

    auto service = ServiceInstance();
    auto result = service->TransferExit(id).GetResult();

    ASSERT_FALSE(result);
    ASSERT_EQ(result.Error(), UCMErrorDomainErrc::kInsufficientData);
}

TEST_F(PackageManagerFixture, GetSwPackages)
{
    // SETUP
    SwPackageInfoType inputData;
    inputData.TransferID = 1;
    inputData.State = SwPackageStateType::kTransferred;
    GetSwPackagesOutput out;
    out.Packages.push_back(inputData);

    EXPECT_CALL(*pmIntMock_, GetSwPackages()).WillOnce(Return(out));

    // TEST
    auto service = ServiceInstance();
    auto result = service->GetSwPackages().get().Packages;

    // ASSERTS
    ASSERT_THAT(result.size(), 1);
    auto data = result[0];

    ASSERT_THAT(data.TransferID, inputData.TransferID);
    ASSERT_THAT(data.State, inputData.State);
    ASSERT_THAT(data.Name, SwInfoName());
    ASSERT_THAT(data.Version, "");
}

TEST_F(PackageManagerFixture, GetLog)
{
    // SETUP
    LogEntryType testRecord;
    testRecord.LogLevel = LogLevelType::kWarning;
    testRecord.Message = "Message";
    LogVectorType test;
    test.push_back(testRecord);

    ara::core::Future<PackageManagement::GetLogOutput>::PromiseType res;
    res.set_value({test});

    EXPECT_CALL(*pmIntMock_, GetLog(id)).WillOnce(Return(ByMove(res.get_future())));

    // TEST
    auto service = ServiceInstance();
    auto result = service->GetLog(id).GetResult();

    // ASSERTS
    ASSERT_TRUE(result.HasValue());
    ASSERT_THAT(result.Value().log.size(), test.size());

    auto record = result.Value().log[0];
    ASSERT_THAT(record.LogLevel, testRecord.LogLevel);
    ASSERT_THAT(record.Message, testRecord.Message);
}

TEST_F(PackageManagerFixture, GetLog_InvalidId)
{
    LogVectorType test;
    ara::core::Future<PackageManagement::GetLogOutput>::PromiseType res;
    res.set_value({test});

    EXPECT_CALL(*pmIntMock_, GetLog(id)).WillOnce(Return(ByMove(res.get_future())));

    // TEST
    auto service = ServiceInstance();
    auto result = service->GetLog(id).GetResult();

    // ASSERTS
    ASSERT_TRUE(result.HasValue());
    ASSERT_THAT(result.Value().log.size(), test.size());
}

TEST_F(PackageManagerFixture, CheckTheCancellationOfProcess)
{
    // the call should not reach the implementation,
    // because it is interrupted immediately
    EXPECT_CALL(*pmIntMock_, DoProcessSwPackage(id)).Times(0);

    // TEST
    auto service = ServiceInstance();

    auto processSwPackageResultFuture = service->ProcessSwPackage(id);
    auto cancelResult = service->Cancel(id);

    // ASSERTS
    ASSERT_THAT(processSwPackageResultFuture.GetResult().Error(), UCMErrorDomainErrc::kProcessSwPackageCancelled);
}


/*
abbas
*/


TEST_F(PackageManagerFixture, CheckcallProcessSwPackage)
{
    // the call should not reach the implementation,
    // because it is interrupted immediately
    EXPECT_CALL(*pmIntMock_,ProcessSwPackage(id)).Times(0);

    // TEST
    auto service = ServiceInstance();

    auto processSwPackageResultFuture = service->ProcessSwPackage(id);
    auto Result = service->ProcessSwPackage(id);

    // ASSERTS
       ASSERT_THAT(processSwPackageResultFuture.GetResult().Error(), UCMErrorDomainErrc::kServiceBusy);

}

TEST_F(PackageManagerFixture, CheckTheProccisingFalierorsuccess)
{
    // the call should not reach the implementation,
    // because it is interrupted immediately
    EXPECT_CALL(*pmIntMock_, DoProcessSwPackage(id)).Times(0);

    // TEST
    auto service = ServiceInstance();

    auto processSwPackageResultFuture = service->ProcessSwPackage(id).GetResult();
    //auto cancelResult = service->Cancel(id);

    // ASSERTS
    ASSERT_FALSE(processSwPackageResultFuture.HasValue());
}
//abbas
TEST_F(PackageManagerFixture, DeleteTransfer_Success)
{
    ara::core::Promise<void> res;
    res.set_value();

    EXPECT_CALL(*pmIntMock_, DeleteTransfer(id)).WillOnce(Return(ByMove(res.get_future())));

    auto service = ServiceInstance();
    auto result = service->DeleteTransfer(id).GetResult();

    ASSERT_TRUE(result.HasValue());
}
/*abbas section*/
/// b3basa gamda


TEST_F(PackageManagerFixture,Activate_kErrorDuringActivation)
{
    ara::core::Promise<void> res;
    res.set_value();
    ActivateOptionType option = ActivateOptionType::kDefault;
    
    EXPECT_CALL(*pmIntMock_, Activate(option)).WillRepeatedly(Return(ByMove(res.get_future())));

    auto service = ServiceInstance();
    auto result = service->Activate(option).GetResult();

    ASSERT_FALSE(result.HasValue());
}





TEST_F(PackageManagerFixture,CancelkCancelFaild)
{
    ara::core::Promise<void> res;
    res.SetError(UCMErrorDomainErrc::kCancelFailed);
    EXPECT_CALL(*pmIntMock_, Cancel(id)).WillRepeatedly(Return(ByMove(res.get_future())));

    auto service = ServiceInstance();
    auto result = service->Cancel(id).GetResult();

    ASSERT_THAT(result.Error(),UCMErrorDomainErrc::kCancelFailed);
}



/*abbas section */




TEST_F(PackageManagerFixture, DeleteTransfer_TransferIdInvalid)
{
    ara::core::Promise<void> res;
    res.SetError(UCMErrorDomainErrc::kInvalidTransferId);

    EXPECT_CALL(*pmIntMock_, DeleteTransfer(id)).WillOnce(Return(ByMove(res.get_future())));

    auto service = ServiceInstance();
    auto result = service->DeleteTransfer(id).GetResult();

    ASSERT_THAT(result.Error(), UCMErrorDomainErrc::kInvalidTransferId);
}

TEST_F(PackageManagerFixture, ReadServiceInstanceID_Success)
{
    PackageManagementApplication appTest;
    EXPECT_EQ(appTest.GetServiceInstanceId(appTest.serviceDeploymentManifestPath_), 828);
}

}  // namespace pkgmgr
}  // namespace ucm
}  // namespace ara
