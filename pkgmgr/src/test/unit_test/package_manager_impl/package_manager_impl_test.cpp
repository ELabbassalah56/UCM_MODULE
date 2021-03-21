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
#include "boost/filesystem.hpp"
#include "filesystem.h"
#include "package_manager_impl/package_manager_impl.h"
#include "test/unit_test/manifest_samples.h"
#include "transfer/software_package_data_factory.h"
#include "transfer/transfer_instance.h"
#include "parsing/application_list_builder.h"
#include "parsing/application.h"
#include "manifest_samples.h"
#include "package_manager_state/package_manager_processing_state.h"
#include "interruptible_thread/interruptible_thread.h"

using ::testing::Eq;
using ::testing::Ne;
using ::testing::NiceMock;
using ::testing::Return;
using ::testing::ReturnRef;
using ::testing::_;

namespace ara
{
namespace ucm
{
namespace pkgmgr
{

const TransferIdType validTestTransferId = 14567;  // this one would succeed, all other fail

class StreamableSoftwarePackageMock : public StreamableSoftwarePackage
{
    Logger logger_;
    TransferIdType unused;
    SwPackageStateType unused2;

public:
    TransferStartReturnType TransferStart(const std::uint32_t& size)
    {
        (void)size;
        return {0, TransferStartSuccessType::kSuccess};
    }
    TransferDataReturnType TransferData(const ByteVectorType& data, const std::uint32_t& blockCounter)
    {
        (void)data;
        (void)blockCounter;
        return TransferDataReturnType::kSuccess;
    }
    TransferExitReturnType TransferExit()
    {
        return TransferExitReturnType::kSuccess;
    }

    TransferIdType const& GetID() const
    {
        return unused;
    }
    void SetState(SwPackageStateType state)
    {
        (void)state;
    }
    SwPackageStateType const& GetState() const
    {
        return unused2;
    }
    Logger& GetLogger()
    {
        return logger_;
    }
    DeleteTransferReturnType DeleteTransfer()
    {
        return DeleteTransferReturnType::kSuccess;
    }

    ara::core::String GetPackagePath() const override
    {
        return kTestInstallSwPackagePath;
    }
    StreamableSoftwarePackageMock()
    {}
};

class SynchronizedStorageStub : public SynchronizedStorage<TransferIdType, StreamableSoftwarePackage>
{
public:
    using Value = std::shared_ptr<StreamableSoftwarePackage>;

    Value GetItem(TransferIdType id) const override
    {
        if (id == validTestTransferId) {
            return std::make_shared<StreamableSoftwarePackageMock>();
        } else {
            return SynchronizedStorage<TransferIdType, StreamableSoftwarePackage>::GetItem(id);
        }
    }

    SynchronizedStorageStub()
    {}

    virtual ~SynchronizedStorageStub()
    {}
};

class PackageManagerImplFixture : public ::testing::Test
{
public:
    PackageManagerImplFixture()
    {}

    /// Note that you need to set any mock expectations
    /// before accessing the package manager via this method
    PackageManagerImpl& packageManagerImpl()
    {
        if (!pm_) {
            pm_ = std::make_unique<PackageManagerImpl>(  //
                std::move(packageExtractor_),
                std::move(packageParser_),
                std::move(swclManager_),
                std::move(processesListManager_),
                std::move(softwarePackageFactory_),
                std::move(packagesData_),
                std::string("/tmp"));
        }
        return *pm_;
    }

    void SetUp()
    {
        packageExtractor_ = std::make_unique<SoftwarePackageExtractorMock>();
        SoftwareClusterManifest swclManifest_ = {"the category",
            "the changes",
            "the license",
            Version("0.5.0"),
            "the shortName",
            "the uuid",
            "the vendorID",
            Version("1.0.0"),
            {"dependentSoftwareClusterA", "dependentSoftwareClusterB", "dependentSoftwareClusterC"}};
        SoftwareCluster swcl = {swclManifest_, ""};
        packageParser_ = std::make_unique<SoftwarePackageParserStub>(CreateDefaultSoftwarePackageManifest("Install"));
        swclManager_ = std::make_unique<SWCLManagerStub>();
        processesListManager_ = std::make_unique<ProcessesListManagerMock>();
        softwarePackageFactory_ = std::make_unique<StreamableSoftwarePackageFactoryStub>();
        packagesData_ = std::make_unique<SynchronizedStorageStub>();
        SoftwarePackageManifest tempManifest = {ActionType::kInstall,
            "the activationAction",
            "the category",
            "the compressedSoftwarePackageSize",
            "the diagnosticAddress",
            "the isDeltaPackage",
            Version("1.0.0"),
            Version("1.0.0"),
            "the packagerID",
            "swcl0",
            "the typeApproval",
            "the ucmIdentifier",
            "the uncompressedSoftwarePackageSize",
            "the uuid"};
        package_ = std::make_unique<SoftwarePackageMock>(tempManifest, "", swcl);
    }

    SoftwareClusterManifest CreateDefaultSWCLManifest()
    {
        return testdata::CreateSoftwareClusterManifestObject("swcl");
    }

    SoftwarePackageManifest CreateDefaultSoftwarePackageManifest(const String& action)
    {
        return testdata::CreateSoftwarePackageManifestObject(action);
    }

    std::unique_ptr<ReversibleActionStub> CreateReversibleAction(Result<void, ErrorCode> result)
    {
        return std::make_unique<ReversibleActionStub>("", *package_, result);
    }

    std::unique_ptr<SoftwarePackageExtractorMock> packageExtractor_;
    std::unique_ptr<SoftwarePackageParserStub> packageParser_;
    std::unique_ptr<SWCLManagerStub> swclManager_;
    std::unique_ptr<ProcessesListManagerMock> processesListManager_;
    std::unique_ptr<StreamableSoftwarePackageFactoryStub> softwarePackageFactory_;
    std::unique_ptr<SynchronizedStorageStub> packagesData_;

    FilesystemMock fs_;
    std::unique_ptr<SoftwarePackageMock> package_;
    const uint64_t swpackage_size = 26042;
    const ara::core::Vector<uint8_t> swpackage_checksum = {0x2F};

private:
    std::unique_ptr<PackageManagerImpl> pm_;
};

TEST_F(PackageManagerImplFixture, SoftwarePackageParsingFailed)
{
    EXPECT_CALL(*packageExtractor_, Cleanup()).Times(1);
    packageParser_->SetInvalidManifestFlag(true);

    auto result = packageManagerImpl().DoProcessSwPackage(validTestTransferId);
   

    ASSERT_THAT(static_cast<UCMErrorDomainErrc>(result.Error().Value()), Eq(UCMErrorDomainErrc::kInvalidManifest));
}

TEST_F(PackageManagerImplFixture, InstallationCommandExecution)
{
    EXPECT_CALL(*packageExtractor_, Cleanup()).Times(1);
    auto action = CreateReversibleAction({});

    auto result = packageManagerImpl().DoProcessSwPackage(validTestTransferId);

    ASSERT_THAT(result.HasValue(), Eq(true));
}

TEST_F(PackageManagerImplFixture, SoftwareDataExistsAfterTransferStart)
{
    std::uint32_t size = 100;
    TransferIdType packageId = 1;

    auto expectedResult = TransferStartReturnType({packageId, TransferStartSuccessType::kSuccess});

    ON_CALL(*softwarePackageFactory_->swPackageMock, GetID()).WillByDefault(ReturnRef(packageId));
    ON_CALL(*softwarePackageFactory_->swPackageMock, TransferStart(_)).WillByDefault(Return(expectedResult));

    packageManagerImpl().TransferStart(size);
    auto packagesData = packageManagerImpl().GetSwPackages().Packages;

    ASSERT_THAT(packagesData.size(), 1);

    auto iterator = std::find_if(packagesData.begin(),
        packagesData.end(),
        [expectedResult](SwPackageInfoType const& t) { return t.TransferID == expectedResult.TransferId; });

    ASSERT_THAT(iterator, Ne(packagesData.end()));
    ASSERT_THAT(iterator->TransferID, expectedResult.TransferId);
    ASSERT_THAT(iterator->State, SwPackageStateType::kTransferring);
}

TEST_F(PackageManagerImplFixture, SoftwareDataHasRightStateAfterTransferExit)
{
    EXPECT_CALL(*packageExtractor_, Extract(_,_)).WillOnce(Return(true));

    auto resultOfTransferExit = packageManagerImpl().TransferExit(validTestTransferId).GetResult();
}

TEST_F(PackageManagerImplFixture, InterruptProcessingWithCancelCommand)
{
    EXPECT_CALL(*packageExtractor_, Cleanup()).Times(1);

    TransferIdType stub_id{0};
    InterruptToken token{false};
    auto state = std::make_unique<PackageManagerProcessingState>(token, stub_id);

    std::promise<void> promise;

    InterruptibleThread th(token,
        [&](std::future<void> provided_future) {
            provided_future.get();
            auto result = packageManagerImpl().DoProcessSwPackage(validTestTransferId);
            ASSERT_THAT(static_cast<UCMErrorDomainErrc>(result.Error().Value()),
                Eq(UCMErrorDomainErrc::kProcessSwPackageCancelled));
        },
        std::move(promise.get_future()));

    state->Cancel(stub_id);
    promise.set_value();

    th.Join();
}

TEST_F(PackageManagerImplFixture, GetHistoryCheckWithFourCorrectElements)
{
    ASSERT_THAT(packageManagerImpl().GetHistory(2346, 123443211).history.size(), Eq(4));
}

}  // namespace pkgmgr
}  // namespace ucm
}  // namespace ara
