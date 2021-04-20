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

#include "package_management_app.h"
#include "ara/core/future.h"
#include "ara/core/promise.h"
#include "ara/core/string.h"

using ara::com::ServiceHandleContainer;
using ara::core::Future;
using ara::core::String;
using ara::ucm::pkgmgr::ActivateOptionType;
using ara::ucm::pkgmgr::PackageManagement;
using ara::ucm::pkgmgr::TransferIdType;
using ara::ucm::pkgmgr::PackageManagerStatusType;
using GetSwPackagesOutput = ara::ucm::pkgmgr::PackageManagement::GetSwPackagesOutput;
namespace ara
{
namespace ucm
{
namespace pkgmgrsample
{

namespace
{
void LogErrorCode(const int errorCode, const String& msg, log::Logger& logger_)
{

    logger_.LogError() << msg << "Error code: " << errorCode;
}
}  // namespace

template <class PackageManagementService>
bool PackageManagementApp<PackageManagementService>::Init(std::shared_ptr<PackageManagementService> service)
{
    service_ = service;

    if (service_) {
        logger_.LogInfo() << "Init success. Service initialized";
        return true;
    } else {
        logger_.LogError() << "Init failed. Service not initialized";
        return false;
    }
}

template <class PackageManagementService>
bool PackageManagementApp<PackageManagementService>::IsCorrectStatus(pkgmgr::PackageManagerStatusType askedStatus)
{
    while (CurrentStatusToString(currentStatus_).size() == 0) {
        logger_.LogInfo() << "Waiting for status field.";
        std::this_thread::sleep_for(std::chrono::milliseconds(400));
    }

    if (currentStatus_ != askedStatus) {
        return false;
    }
    return true;
}

template <class PackageManagementService>
void PackageManagementApp<PackageManagementService>::OnStatusChange()
{
    std::unique_lock<std::mutex> lck(globalMutex);
    service_->CurrentStatus.Update();
    const auto& state = service_->CurrentStatus.GetCachedSamples();
    if (!state.empty()) {
        this->currentStatus_ = **state.begin();

        if (CurrentStatusToString(currentStatus_).size() > 0) {
            logger_.LogInfo() << "Server state transition notification :" << CurrentStatusToString(currentStatus_);
        } else {
            logger_.LogError() << "Empty status field.";
        }
        service_->CurrentStatus.Cleanup();
        cond.notify_one();
    }
}

template <class PackageManagementService>
bool PackageManagementApp<PackageManagementService>::UpdateStatus()
{
    if (service_->CurrentStatus.IsSubscribed()) {
        logger_.LogError() << "status field callback already subscribed";
        return false;
    }

    // Register event receive callback
    service_->CurrentStatus.SetReceiveHandler(
        [this]() { PackageManagementApp<PackageManagementService>::OnStatusChange(); });

    // Subscribe to field
    service_->CurrentStatus.Subscribe(ara::com::EventCacheUpdatePolicy::kNewestN, 1);

    // Initial update of service.
    // This is necessary in case application is being re-attached to the service
    // after a crash.
    service_->CurrentStatus.Update();
    auto state = service_->CurrentStatus.Get();
    this->currentStatus_ = state.get();
    service_->CurrentStatus.Cleanup();

    // service_ got initialized via callback.
    logger_.LogInfo() << "Status field callback registered.";
    return true;
}

template <class PackageManagementService>
bool PackageManagementApp<PackageManagementService>::TransferSoftwarePackage(const ara::core::String& packagePath,
    DataTransfer<PackageManagementService>& dataTransfer)
{
    std::vector<uint8_t> buffer;
    bool found = LoadBinaryFile(packagePath, buffer);
    if (!found) {
        logger_.LogError() << "Transfer failed. Software Package not found at path " << packagePath;
        return false;
    }

    if (!dataTransfer.InitSession(buffer.size())) {
        logger_.LogError() << "Initialization of transfer with size " + std::to_string(buffer.size()) + " failed";
        return false;
    }

    lastTransferId_ = dataTransfer.GetTransferId();

    if (!dataTransfer.Transfer(buffer)) {
        logger_.LogError() << "Transfer of software package associated with TransferId " << lastTransferId_
                           << " failed";
        return false;
    }

    if (!dataTransfer.ExitSession()) {
        logger_.LogError() << "Exiting transfer associated with TransferId " << lastTransferId_ << " failed";
        return false;
    }
    logger_.LogInfo() << "Transfer of Software Package was successful for session " << lastTransferId_;
    return true;
}

template <class PackageManagementService>
bool PackageManagementApp<PackageManagementService>::TransferSoftwarePackage(const ara::core::String& packagePath)
{

    DataTransfer<PackageManagementService> dataTransfer(service_, 128);
    return this->TransferSoftwarePackage(packagePath, dataTransfer);
}

template <class PackageManagementService>
bool PackageManagementApp<PackageManagementService>::DeleteSoftwarePackages()
{
    DataTransfer<PackageManagementService> dataTransfer(service_, 128);
    GetSwPackagesOutput getSwPackageResult = dataTransfer.GetSwPackages();

    ara::ucm::pkgmgr::TransferIdType id;
    for (auto entry : getSwPackageResult.Packages) {
        id = entry.TransferID;
        bool deleteTransferResult = dataTransfer.DeleteSwPackage(id);

        if (!deleteTransferResult) {
            LogErrorCode(static_cast<int>(deleteTransferResult), "Deleting transfer failed", logger_);
            logger_.LogInfo() << "Delete package failed for TransferID " << id;
        }
    }

    getSwPackageResult = dataTransfer.GetSwPackages();
    if (getSwPackageResult.Packages.size() != 0) {
        logger_.LogInfo() << "Package deletion failed."
                          << "Remaining packages: " << static_cast<int>(getSwPackageResult.Packages.size());
        return false;
    }
    return true;
}

template <class PackageManagementService>
bool PackageManagementApp<PackageManagementService>::Install()
{
    std::cout<<"Invoke ProcessSWPackage"<<std::endl;

    auto result = service_->ProcessSwPackage(lastTransferId_).GetResult();
    
    if (result.HasValue()) {
        logger_.LogInfo() << "Processing of the processed software package was successful";
        return true;
    } else {
        logger_.LogError() << "Processing of software package failed:" << result.Error().Message();
        return false;
    }
}

template <class PackageManagementService>
bool PackageManagementApp<PackageManagementService>::Activate()
{
    ActivateOptionType option = ActivateOptionType::kDefault;
    auto result = service_->Activate(option).GetResult();
    if (result.HasValue()) {
        logger_.LogInfo() << "Activation of the processed software package was successful";
        return true;
    } else {
        logger_.LogError() << "Activation of software package failed:" << result.Error().Message();
        return false;
    }
}

template <class PackageManagementService>
bool PackageManagementApp<PackageManagementService>::Finish()
{
    auto result = service_->Finish().GetResult();
    if (result.HasValue()) {
        logger_.LogInfo() << "Finalization of the processed software package was successful";
        return true;
    } else {
        logger_.LogError() << "Finalization of software package failed:" << result.Error().Message();
        return false;
    }
}

template <class PackageManagementService>
bool PackageManagementApp<PackageManagementService>::Rollback()
{
    auto result = service_->Rollback().GetResult();
    if (result.HasValue()) {
        logger_.LogInfo() << "Finalization of the processed software package was successful";
        return true;
    } else {
        logger_.LogError() << "Finalization of software package failed:" << result.Error().Message();
        return false;
    }
}

template <class PackageManagementService>
bool PackageManagementApp<PackageManagementService>::Cancel(ara::ucm::pkgmgr::TransferIdType processingTransferId)
{
    logger_.LogInfo() << "Cancel initiated ";
    auto result = service_->Cancel(processingTransferId).GetResult();
    if (result.HasValue()) {
        logger_.LogInfo() << "Cancel of software package was successful ";
        return true;
    } else {
        logger_.LogError() << "Cancel of software package ID =" << processingTransferId
                           << " failed:" << result.Error().Message();
        return false;
    }
}

template <class PackageManagementService>
bool PackageManagementApp<PackageManagementService>::RevertProcessedSwPackages()
{
    logger_.LogInfo() << "RevertProcessedSwPackages initiated ";
    auto result = service_->RevertProcessedSwPackages().GetResult();
    if (result.HasValue()) {
        logger_.LogInfo() << "Revert of software package was successful ";
        return true;
    } else {
        logger_.LogError() << "Finalization of software package failed:" << result.Error().Message();
        return false;
    }
}

template <class PackageManagementService>
bool PackageManagementApp<PackageManagementService>::AttemptStateRecovery()
{
    bool result = false;
    std::unique_lock<std::mutex> lck(globalMutex, std::defer_lock);

    switch (currentStatus_) {

    case pkgmgr::PackageManagerStatusType::kReady: {
        logger_.LogInfo() << "Attempting pkgmanagement state recovery: kReady";
        return this->RevertProcessedSwPackages();
    }

    case pkgmgr::PackageManagerStatusType::kProcessing: {
        logger_.LogInfo() << "Attempting pkgmanagement state recovery: kProcessing";
        ara::ucm::pkgmgr::TransferIdType processingTransferId = GetProcessingTransferId();
        bool cancelSucceeded = this->Cancel(processingTransferId);
        if (cancelSucceeded && (this->cond.wait_for(lck, std::chrono::milliseconds(10000), [&] {
                return this->IsCorrectStatus(ara::ucm::pkgmgr::PackageManagerStatusType::kReady);
            }))) {
            result = true;
        } else {
            result = false;
        }
    } break;

    case pkgmgr::PackageManagerStatusType::kActivating:
        logger_.LogInfo() << "Attempting pkgmanagement state recovery: kActivating";
        if (this->cond.wait_for(lck, std::chrono::milliseconds(10000), [&] {
                return this->IsCorrectStatus(ara::ucm::pkgmgr::PackageManagerStatusType::kVerifying);
            })) {
            result = this->AttemptStateRecovery();
        } else {
            result = false;
        }
        break;

    case pkgmgr::PackageManagerStatusType::kVerifying:
        logger_.LogInfo() << "Attempting pkgmanagement state recovery: kVerifying";
        if (this->Rollback() && this->cond.wait_for(lck, std::chrono::milliseconds(10000), [&] {
                return this->IsCorrectStatus(ara::ucm::pkgmgr::PackageManagerStatusType::kRollingBack);
            })) {
            result = this->AttemptStateRecovery();
        } else {
            result = false;
        }
        break;

    case pkgmgr::PackageManagerStatusType::kActivated:
        logger_.LogInfo() << "Attempting pkgmanagement state recovery: kActivated";
        if (this->Rollback() && this->cond.wait_for(lck, std::chrono::milliseconds(10000), [&] {
                return this->IsCorrectStatus(ara::ucm::pkgmgr::PackageManagerStatusType::kRollingBack);
            })) {
            result = this->AttemptStateRecovery();
        } else {
            result = false;
        }
        break;

    case pkgmgr::PackageManagerStatusType::kRollingBack:
        logger_.LogInfo() << "Attempting pkgmanagement state recovery: kRollingBack";
        if (this->cond.wait_for(lck, std::chrono::milliseconds(10000), [&] {
                return this->IsCorrectStatus(ara::ucm::pkgmgr::PackageManagerStatusType::kRolledBack);
            })) {
            result = this->AttemptStateRecovery();
        } else {
            result = false;
        }
        break;

    case pkgmgr::PackageManagerStatusType::kRolledBack:
        logger_.LogInfo() << "Attempting pkgmanagement state recovery: kRolledBack";
        if (this->Finish() && this->cond.wait_for(lck, std::chrono::milliseconds(10000), [&] {
                return this->IsCorrectStatus(ara::ucm::pkgmgr::PackageManagerStatusType::kCleaningUp);
            })) {
            result = this->AttemptStateRecovery();
        } else {
            result = false;
        }
        break;

    case pkgmgr::PackageManagerStatusType::kCleaningUp:
        logger_.LogInfo() << "Attempting pkgmanagement state recovery: kCleaningUp";
        result = this->cond.wait_for(lck, std::chrono::milliseconds(10000), [&] {
            return this->IsCorrectStatus(ara::ucm::pkgmgr::PackageManagerStatusType::kReady);
        });
        break;

    default:
        result = false;
        break;
    }

    return result;
}

template <class PackageManagementService>
bool PackageManagementApp<PackageManagementService>::AreChangesPending()
{
    Future<PackageManagement::GetSwClusterChangeInfoOutput> futureChangeInfoOutput = service_->GetSwClusterChangeInfo();
    PackageManagement::GetSwClusterChangeInfoOutput changeInfoOutput = futureChangeInfoOutput.get();
    return !(changeInfoOutput.SwInfo.empty());
}

template <class PackageManagementService>
ara::ucm::pkgmgr::TransferIdType PackageManagementApp<PackageManagementService>::GetProcessingTransferId()
{
    Future<PackageManagement::GetSwPackagesOutput> futureGetPackagesOutput = service_->GetSwPackages();
    PackageManagement::GetSwPackagesOutput GetPackagesOutput = futureGetPackagesOutput.get();

    // In case no packages are being processed, then return an error.
    // ToDo Error to be implemented with the implementation of error domains
    ara::ucm::pkgmgr::TransferIdType activeTransferId = 0;
    for (ara::ucm::pkgmgr::SwPackageInfoType package : GetPackagesOutput.Packages) {
        if (package.State == ara::ucm::pkgmgr::SwPackageStateType::kProcessing) {
            activeTransferId = package.TransferID;
            break;
        }
    }

    return activeTransferId;
}

bool LoadBinaryFile(const ara::core::String& filePath, std::vector<uint8_t>& binaryPackage)
{
    bool fileFound = false;
    std::ifstream ifs;
    ifs.open(filePath.c_str(), std::ios::binary | std::ios::in);
    if (ifs.is_open()) {
        char c;
        while (ifs.get(c)) {
            binaryPackage.push_back(c);
        }

        fileFound = true;
    }
    ifs.close();

    return fileFound;
}

const ara::core::String CurrentStatusToString(pkgmgr::PackageManagerStatusType status)
{
    switch (status) {
    case pkgmgr::PackageManagerStatusType::kProcessing:
        return "Processing";

    case pkgmgr::PackageManagerStatusType::kReady:
        return "Ready";

    case pkgmgr::PackageManagerStatusType::kActivating:
        return "Activating";

    case pkgmgr::PackageManagerStatusType::kVerifying:
        return "Verifying";

    case pkgmgr::PackageManagerStatusType::kActivated:
        return "Activated";

    case pkgmgr::PackageManagerStatusType::kRollingBack:
        return "Rolling Back";

    case pkgmgr::PackageManagerStatusType::kRolledBack:
        return "Rolled Back";

    case pkgmgr::PackageManagerStatusType::kCleaningUp:
        return "Cleaning Up";

    default:
        return "Unknown";
    }
}

template class PackageManagementApp<ara::ucm::pkgmgr::proxy::PackageManagementProxy>;
}  // namespace pkgmgrsample
}  // namespace ucm
}  // namespace ara
