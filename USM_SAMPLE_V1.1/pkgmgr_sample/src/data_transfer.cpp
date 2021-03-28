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

#include "data_transfer.h"
#include "ara/core/future.h"
#include "ara/core/promise.h"

using ara::core::Future;
using ara::core::Promise;
using ara::ucm::pkgmgr::PackageManagement;
using ara::ucm::pkgmgr::TransferIdType;
using GetSwPackagesOutput = ara::ucm::pkgmgr::PackageManagement::GetSwPackagesOutput;

namespace ara
{
namespace ucm
{
namespace pkgmgrsample
{

// helper methods
uint32_t CalculateBlockCount(uint32_t dataAmountInBytes, uint32_t blockSizeInBytes);
void InitDataBlock(const uint32_t startIndex,
    const uint32_t blockSize,
    const std::vector<uint8_t>& data,
    std::vector<uint8_t>& dataBlock);

template <class PackageManagementService>
DataTransfer<PackageManagementService>::DataTransfer(std::shared_ptr<PackageManagementService> packageManagementService,
    uint32_t blockSize)
    : packageManagementService_(packageManagementService)
    , blockSize_(blockSize)
    , currentSession_(0)
    , logger_(log::CreateLogger("DT", "DataTransfer", log::LogLevel::kVerbose))
{}

template <class PackageManagementService>
bool DataTransfer<PackageManagementService>::InitSession(uint32_t size)
{
    Future<PackageManagement::TransferStartOutput> transferStartFuture = packageManagementService_->TransferStart(size);

    auto transferStartResult = transferStartFuture.GetResult();

    if (!transferStartResult) {
        logger_.LogError() << "Error while initializing transfer session "
                           << ". Error message: " << transferStartResult.Error().Message();
        return false;
    } else {
        currentSession_ = transferStartResult.Value().id;
        logger_.LogInfo() << "InitSession was successful for session " << currentSession_;
        return true;
    }
}

template <class PackageManagementService>
bool DataTransfer<PackageManagementService>::Transfer(const std::vector<uint8_t>& data)
{
    uint32_t blockCount = CalculateBlockCount(data.size(), blockSize_);

    uint32_t blocksSent = 0;

    while (blocksSent < blockCount) {
        std::vector<uint8_t> dataBlock;
        InitDataBlock(blocksSent * blockSize_, blockSize_, data, dataBlock);

        auto transferDataFuture = packageManagementService_->TransferData(currentSession_, dataBlock, blocksSent + 1);

        auto transferDataResult = transferDataFuture.GetResult();
        if (!transferDataResult) {
            logger_.LogError() << "Error while sending data block number " << blocksSent << " in session "
                               << currentSession_ << ". Error: " << transferDataResult.Error().Message();
            return false;
        }
        ++blocksSent;
    }
    logger_.LogInfo() << "Successful transfer of data of size " << data.size() << " with block count " << blocksSent
                      << " in session " << currentSession_;
    return true;
}

template <class PackageManagementService>
bool DataTransfer<PackageManagementService>::ExitSession()
{
    auto transferExitResult = packageManagementService_->TransferExit(currentSession_).GetResult();

    if (!transferExitResult) {
        logger_.LogError() << "Error while exiting session " << currentSession_
                           << ". Error: " << transferExitResult.Error().Message();
        return false;
    }
    logger_.LogInfo() << "Session exited successfully for session " << currentSession_ << " .";
    return true;
}

template <class PackageManagementService>
bool DataTransfer<PackageManagementService>::DeleteSwPackage(const ara::ucm::pkgmgr::TransferIdType transferId)
{
    Future<void> deleteTransferFuture = packageManagementService_->DeleteTransfer(transferId);

    auto deleteTransferResult = deleteTransferFuture.GetResult();

    if (!deleteTransferResult) {
        logger_.LogError() << "Error while delete transfer " << transferId
                           << ". Error messagge: " << deleteTransferResult.Error().Message();
        return false;
    } else {
        logger_.LogInfo() << "Deleted package successfully for TransferID " << transferId << " .";
        return true;
    }
}

template <class PackageManagementService>
GetSwPackagesOutput DataTransfer<PackageManagementService>::GetSwPackages()
{
    Future<PackageManagement::GetSwPackagesOutput> getSwPackagesFuture = packageManagementService_->GetSwPackages();

    PackageManagement::GetSwPackagesOutput getSwPackagesOutput = getSwPackagesFuture.get();

    logger_.LogInfo() << "Get software packages successfully."
                      << "the number of packages: " << static_cast<int>(getSwPackagesOutput.Packages.size());
    return getSwPackagesOutput;
}

template <class PackageManagementService>
TransferIdType DataTransfer<PackageManagementService>::GetTransferId()
{
    return currentSession_;
}

template <class PackageManagementService>
void DataTransfer<PackageManagementService>::SetBlockSize(uint32_t size)
{
    blockSize_ = size;
}

uint32_t CalculateBlockCount(uint32_t dataAmountInBytes, uint32_t blockSizeInBytes)
{
    uint32_t blockCount = dataAmountInBytes / blockSizeInBytes;
    if (dataAmountInBytes % blockSizeInBytes != 0) {
        blockCount++;
    }
    return blockCount;
}

void InitDataBlock(const uint32_t startIndex,
    const uint32_t blockSize,
    const std::vector<uint8_t>& data,
    std::vector<uint8_t>& dataBlock)
{
    for (uint32_t i = startIndex; i < startIndex + blockSize; i++) {
        if (i < data.size()) {
            dataBlock.push_back(data[i]);
        } else {
            break;
        }
    }
}

template class DataTransfer<ara::ucm::pkgmgr::proxy::PackageManagementProxy>;

}  // namespace pkgmgrsample
}  // namespace ucm
}  // namespace ara
