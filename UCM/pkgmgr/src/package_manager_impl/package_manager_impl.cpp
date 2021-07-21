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

#include "package_manager_impl/package_manager_impl.h"

#include "transfer/receive_types.h"
#include "transfer/software_package_data.h"

#include "extraction/software_package_extractor.h"

#include "parsing/software_package_parser.h"
#include "parsing/software_package.h"
#include "parsing/software_cluster_manifest.h"
#include "parsing/ucm_strings.h"
#include "exceptions.h"

#include "interruptible_thread/interruptible_thread.h"

#include "storage/action_generator.h"

#include <stdlib.h>

namespace ara
{
namespace ucm
{
namespace pkgmgr
{

PackageManagerImpl::PackageManagerImpl(std::unique_ptr<SoftwarePackageExtractor> softwarePackageExtractor,
    std::unique_ptr<SoftwarePackageParser> softwarePackageParser,
    std::unique_ptr<SWCLManager> SWCLManager,
    std::unique_ptr<ProcessesListManager> processesListManager,
    std::unique_ptr<StreamableSoftwarePackageFactory> packageDataFactory,
    std::unique_ptr<SynchronizedStorage<TransferIdType, StreamableSoftwarePackage>> packagesData,
    const core::String& installDirectory)
    : softwarePackageExtractor_(std::move(softwarePackageExtractor))
    , softwarePackageParser_(std::move(softwarePackageParser))
    , SWCLManager_(std::move(SWCLManager))
    , processesListManager_(std::move(processesListManager))
    , softwarePackageFactory_(std::move(packageDataFactory))
    , packagesData_(std::move(packagesData))
    , installDirectory_(installDirectory)
{}

PackageManagerImpl::~PackageManagerImpl() = default;

Result<void> PackageManagerImpl::ProcessSoftwarePackage(Streamable& package)
{
    


    std::cout<<"\n\n\nProcessing SW PAkage INVOKE \n\n\n"<<std::endl;

    auto path = package.GetPackagePath();

    log_.LogInfo() << "Processing software package at path " << path;

    package.SetProcessProgressValue(0);

    if (InterruptibleThread::IsInterrupted()) {
        // "Cancel" call from client 
        softwarePackageExtractor_->Cleanup();
        return Result<void>::FromError(UCMErrorDomainErrc::kProcessSwPackageCancelled);
    }

    std::unique_ptr<SoftwarePackage> softwarePackage;
    try {
        softwarePackage = softwarePackageParser_->Parse(GetExtractionTargetDirectory(package.GetPackagePath()));
    } catch (const exception::InvalidManifestException& e) {
        softwarePackageExtractor_->Cleanup();
        package.SetProcessProgressValue(100);
        log_.LogWarn() << "Invalid Manifest.";
        return Result<void>::FromError(UCMErrorDomainErrc::kInvalidManifest);
    }

    //Check Version Of Software Cluster 
    try
    {
        auto currAppVersion = softwarePackage->GetSoftwareCluster().GetSwclManifest().GetVersion().ToString();
        auto prevAppVersion = softwarePackage->GetSoftwareCluster().GetSwclManifest().GetPreviousVersion().ToString();
             
        if(versionCompare(currAppVersion,prevAppVersion) < 0)
        {
            throw currAppVersion;
        }
        
    }
    catch(const ara::core::String& e)
    {
       softwarePackageExtractor_->Cleanup();
       package.SetProcessProgressValue(100);
       log_.LogWarn() << "The version of the Software Cluster is smaller than previous version."<<e;
        return Result<void>::FromError(UCMErrorDomainErrc::kOldVersion);
    }

    //Check Version of Software Cluster is Compatabile with Current UCM 
    try
    {
        auto ucmDepend = softwarePackage->GetManifest().GetMinUCMSupportedVersion().ToString();
        auto ucmVer = ucmServiceVersion;
        if(versionCompare(ucmDepend,ucmVer) < 0)
        {
            throw ucmDepend;
        }
        
    }
    catch(const ara::core::String& e)
    {
       softwarePackageExtractor_->Cleanup();
       package.SetProcessProgressValue(100);
       log_.LogWarn() << "The version of the Software Cluster is in compatible wtih UCM version."<<e;
        return Result<void>::FromError(UCMErrorDomainErrc::kIncompatiblePackageVersion);
    }
    
    
   
    if (InterruptibleThread::IsInterrupted()) {
        // "Cancel" call from client
        softwarePackageExtractor_->Cleanup();
        return Result<void>::FromError(UCMErrorDomainErrc::kProcessSwPackageCancelled);
    }

    // Create, execute and maybe store the action to make it possible to revert or commit it later
    {
         std::cout<<"\n\n\nProcessing SW PAkage INVOKE Action Take  \n\n\n"<<std::endl;

        std::unique_ptr<ReversibleAction> currentAction = ActionGenerator()(installDirectory_, *softwarePackage);
        if (currentAction) {
            Result<void> result = currentAction->Execute();

            std::cout<<"\n\n\nProcessing SW PAkage INVOKE ACtion : "<<"\n\n\n"<<std::endl;
            
            
            if (result.HasValue()) {

                SWCLManager_->AddSWCLChangeInfo(*softwarePackage);
              
                for( const auto& index : SWCLManager_->GetSWCLsForActivation() )
                {
                    switch (index.GetSwclState()) 
                    {
                        case SwClusterStateType::kAdded :
                                currentAction->CommitChanges();
                                swClInfo_->SetSwclState (SwClusterStateType::kPresent); 
                                break;
                        case SwClusterStateType::kRemoved :
                                currentAction->CommitChanges();
                                break;
                        case SwClusterStateType::kUpdated :
                                currentAction->CommitChanges();
                                swClInfo_->SetSwclState (SwClusterStateType::kPresent); 
                                break;
                        default:
                                break;
                    } 
                }
                executedActions_.push_back(std::move(currentAction));
            } else {
                for( const auto& index : SWCLManager_->GetSWCLsForActivation())
                {
                    switch (index.GetSwclState()) 
                    {
                        case SwClusterStateType::kAdded :
                                currentAction->RevertChanges();
                                SWCLManager_ = nullptr;
                                currentAction->CommitChanges();
                                swClInfo_->SetSwclState (SwClusterStateType::kPresent); 
                                break;
                        case SwClusterStateType::kRemoved :
                                currentAction->RevertChanges();
                                swClInfo_->SetSwclState (SwClusterStateType::kPresent);
                                currentAction->CommitChanges();
                                break;
                        case SwClusterStateType::kUpdated :
                                currentAction->RevertChanges();
                                swClInfo_->SetSwclState (SwClusterStateType::kPresent); 
                                break;
                        default:
                           break;
                    }

                }
            }
        } else {
            // action is not created, because its type is unknown
            std::cout<<"\n\n******************  ERROR NOT FOUND 404 *****************\n\n\t\t"<<std::endl;        
            log_.LogError() << "Action is not supported";
            std::cout<<"\n\n******************  ERROR NOT FOUND 404 *****************\n\n"<<std::endl;        
      
        }
    }
    
    

    package.SetProcessProgressValue(75);

    softwarePackageExtractor_->Cleanup();

    package.SetProcessProgressValue(100);
     // cancle success
    return {};
}

Result<void> PackageManagerImpl::DoProcessSwPackage(const TransferIdType& id)
{
    auto package = packagesData_->GetItem(id);
    if (!package) {
        return Result<void>::FromError(UCMErrorDomainErrc::kInvalidTransferId);
    } else {
        return ProcessSoftwarePackage(*package);  
    }
}
core::String PackageManagerImpl::GetExtractionTargetDirectory(const core::String& fullFilePath) const
{
    const auto delim = "/";
    core::String filename = fullFilePath.substr(fullFilePath.find_last_of(delim));
    core::String folder = fullFilePath.substr(0, fullFilePath.size() - filename.size());
    core::String extension = ".zip";
    filename = filename.substr(1, filename.size() - extension.size() - 1);
    core::String targetFolder = folder + "/" + filename;
    return targetFolder;
}

TransferDataFuture PackageManagerImpl::TransferData(const TransferIdType& id,
    const ByteVectorType& data,
    const std::uint32_t& blockCounter)
{
    TransferDataFuture::PromiseType promise;
    auto future = promise.get_future();

    auto getItemResult = packagesData_->GetItem(id);

    if (!getItemResult) {
        promise.SetError(UCMErrorDomainErrc::kInvalidTransferId);
        return future;
    } else {
        switch (getItemResult->TransferData(data, blockCounter)) {
        case TransferDataReturnType::kSuccess:
            promise.set_value();
            break;
        case TransferDataReturnType::kIncorrectBlock:
            promise.SetError(UCMErrorDomainErrc::kIncorrectBlock);
            break;
        case TransferDataReturnType::kIncorrectSize:
            promise.SetError(UCMErrorDomainErrc::kIncorrectSize);
            break;
        case TransferDataReturnType::kInsufficientMemory:
            promise.SetError(UCMErrorDomainErrc::kInsufficientMemory);
            break;
        case TransferDataReturnType::kInvalidTransferId:
            promise.SetError(UCMErrorDomainErrc::kInvalidTransferId);
            break;
        default:
            promise.SetError(UCMErrorDomainErrc::kOperationNotPermitted);
        }
    }
    return future;
}

TransferExitFuture PackageManagerImpl::TransferExit(const TransferIdType& id)
{
    Promise<void> promise;
    auto future = promise.get_future();

    auto packagePtr = packagesData_->GetItem(id);

    if (!packagePtr) {
        promise.SetError(UCMErrorDomainErrc::kInvalidTransferId);
        return future;
    }

    //  TBD: This complete switch should disappear as soon as a (legacy) TransferExitReturnType is removed.
    switch (packagePtr->TransferExit()) {
    case TransferExitReturnType::kSuccess:
        packagePtr->SetState(SwPackageStateType::kTransferred);
        {
            std::thread t(
                [&](Promise<void> lambdaPromise) {
                    auto packagePath = packagesData_->GetItem(id)->GetPackagePath();
                    const core::String extractionDirectory = GetExtractionTargetDirectory(packagePath);
                    //  Extract the SWCL if possible
                    if (!softwarePackageExtractor_->Extract(packagePath, extractionDirectory)) {
                        lambdaPromise.SetError(UCMErrorDomainErrc::kPackageInconsistent);
                        return;
                    }
                    // TransferExit() must check for "kInvalidManifest" error
                    try {
                        (void)softwarePackageParser_->Parse(extractionDirectory);
                        lambdaPromise.set_value();
                    } catch (const exception::InvalidManifestException& e) {
                        softwarePackageExtractor_->Cleanup();
                        log_.LogWarn() << "Failed to parse the software package manifest";
                        lambdaPromise.SetError(UCMErrorDomainErrc::kInvalidManifest);
                    }
                },
                std::move(promise));
            t.detach();
        }
        break;
    case TransferExitReturnType::kInsufficientData:
        promise.SetError(UCMErrorDomainErrc::kInsufficientData);
        break;
    case TransferExitReturnType::kPackageInconsistent:
        promise.SetError(UCMErrorDomainErrc::kPackageInconsistent);
        break;
    case TransferExitReturnType::kInvalidTransferId:
        promise.SetError(UCMErrorDomainErrc::kInvalidTransferId);
        break;
    case TransferExitReturnType::kOperationNotPermitted:
        promise.SetError(UCMErrorDomainErrc::kOperationNotPermitted);
        break;
    default:
        promise.SetError(UCMErrorDomainErrc::kOperationNotPermitted);
    }

    return future;
}

TransferStartFuture PackageManagerImpl::TransferStart(const std::uint32_t& size)
{
    TransferStartFuture::PromiseType promise;
    auto future = promise.get_future();

    auto item = packagesData_->CreateAndStoreItem(*softwarePackageFactory_, generator_);

    if (!item) {
        promise.SetError(UCMErrorDomainErrc::kInsufficientMemory);
        return future;
    }

    auto package = *item;

    auto result = package.second->TransferStart(size);

    if (result.TransferStartSuccess == TransferStartSuccessType::kSuccess) {
        package.second->SetState(SwPackageStateType::kTransferring);
        promise.set_value({result.TransferId});
        return future;
    } else {
        packagesData_->DeleteItem(package.first);
        promise.SetError(UCMErrorDomainErrc::kInsufficientMemory);
        return future;
    }
}

GetSwPackagesOutput PackageManagerImpl::GetSwPackages() const
{
    GetSwPackagesOutput out;

    packagesData_->IterateItems([&](const std::pair<TransferIdType, std::shared_ptr<StreamableSoftwarePackage>> pair) {
        SwPackageInfoType info;

        info.TransferID = pair.first;
        info.State = pair.second->GetState();
        // The package in state of "Transferring" has no unpacked manifests
        if (info.State != SwPackageStateType::kTransferring) {
            try {
                auto packagePtr
                    = softwarePackageParser_->Parse(GetExtractionTargetDirectory(pair.second->GetPackagePath()));
                info.Name = packagePtr->GetSoftwareCluster().GetSwclManifest().GetShortName();
                info.Version = packagePtr->GetSoftwareCluster().GetSwclManifest().GetVersion().ToString();
                info.Size = std::atoi(packagePtr->GetManifest().GetCompressedSoftwarePackageSize().c_str());
            } catch (exception::InvalidManifestException& e) {
                log_.LogError() << "GetSwPackages failed to parse manifest" << e.what();
            }
        }

        out.Packages.push_back(info);
    });

    return out;
}

SwClusterInfoVectorType PackageManagerImpl::GetSwClusterInfo() const
{
    SwClusterInfoVectorType out;

    auto data = SWCLManager_->GetPresentSWCLs();
    std::transform(data.begin(), data.end(), std::back_inserter(out), [](SoftwareCluster const& swcl) {
        SwClusterInfoType swclInfo;

        swclInfo.Name = swcl.GetSwclManifest().GetShortName();
        swclInfo.Version = swcl.GetSwclManifest().GetVersion().ToString();
        swclInfo.State = swcl.GetSwclState();

        return swclInfo;
    });

    return out;
}

UCMInfoVectorOfStringType PackageManagerImpl::UCMInfoService() const
{

    UCMInfoVectorOfStringType out;
    UCMInfoType ucmInfo;
    ucmInfo.NAME = ucmInfo_->GetNameUcmInfo();
    ucmInfo.Version = ucmInfo_->GetVersionUcmInfo();
    ucmInfo.RELEASE = ucmInfo_->GetReleaseUcmInfo();
    out.push_back(ucmInfo);

    return out;
}
 

const SwClusterInfoVectorType& PackageManagerImpl::GetSwClusterChangeInfo() const
{
    return SWCLManager_->GetSWCLsChangeInfo();
}
//complete function
GetSwProcessProgressFuture PackageManagerImpl::GetSwProcessProgress(const TransferIdType& id) const
{
    GetSwProcessProgressFuture::PromiseType promise;

    auto package = packagesData_->GetItem(id);
    if (!package) {
        promise.SetError(UCMErrorDomainErrc::kInvalidTransferId);
    } else {
        promise.set_value({package->GetProcessProgressValue()});
    }
    return promise.get_future();
}

ara::core::Future<PackageManagement::GetLogOutput> PackageManagerImpl::GetLog(const TransferIdType id) const
{
    ara::core::Future<PackageManagement::GetLogOutput>::PromiseType promise;

    auto item = packagesData_->GetItem(id);
    if (item == nullptr) {
        promise.SetError(UCMErrorDomainErrc::kInvalidTransferId);
    } else {
        promise.set_value({item->GetLogger().GetLogs()});
    }

    return promise.get_future();
}

std::unique_ptr<PackageManagerState> PackageManagerImpl::RetrieveState(void)
{
    // Check if there is a saved state,
    // if not return Idle state object
    log_.LogInfo() << "Package Manager Idle State Recovered";
    return std::make_unique<PackageManagerIdleState>();
}

GetHistoryOutput PackageManagerImpl::GetHistory(const std::uint64_t& timestampGE, const std::uint64_t& timestampLT)
{
    GetHistoryOutput history;
    // The placement of the DB should be clarified yet
    auto db = ara::per::OpenKeyValueStorage("/usr/share/apdtest/ucm/GetHistoryTestDatabase.json").ValueOrThrow();
    auto db_keys{db->GetAllKeys().ValueOrThrow()};
    for (auto const& storedKey : db_keys) {
        std::uint64_t keyValue;
        char const* rawStringPointer = storedKey.c_str();
        if (rawStringPointer != NULL) {
            char* end;
            keyValue = strtoul(rawStringPointer, &end, 10);
            if (errno == ERANGE) {
                // TBD: when switched to the new interface with Futures, we will set error here
                return {};
            }
        }

        if ((keyValue >= timestampGE) && (keyValue < timestampLT)) {
            ara::core::String storedValue;
            db->GetValue(storedKey, storedValue);
            // One item holds a vector which would look like this:
            // [key:\"2345\",value:\"2345|name1|1.0.1.3|1|0|2345|name2|1.0.2.3|0|1\"]
            std::istringstream inputStream(std::string(storedValue.c_str()));
            std::string temporaryString;
            while (std::getline(inputStream, temporaryString, '|')) {
                GetHistoryType historyRecord;
                historyRecord.Time = std::stoull(temporaryString);
                std::getline(inputStream, temporaryString, '|');
                historyRecord.Name = temporaryString;
                std::getline(inputStream, temporaryString, '|');
                historyRecord.Version = temporaryString;
                std::getline(inputStream, temporaryString, '|');
                historyRecord.Action = static_cast<::ara::ucm::pkgmgr::ActionType>(std::stoi(temporaryString));
                std::getline(inputStream, temporaryString, '|');
                historyRecord.Resolution = static_cast<::ara::ucm::pkgmgr::ResultType>(std::stoi(temporaryString));
                history.history.push_back(historyRecord);
            }  // while
        }  // if
    }  // for
    return history;
}

void PackageManagerImpl::UpdateProcessesList()
{
    processesListManager_->UpdateProcessesList(SWCLManager_->GetSWCLsForActivation());
}

ara::core::Future<void> PackageManagerImpl::DeleteTransfer(const TransferIdType& id)
{
    ara::core::Promise<void> promise;
    auto getItemResult = packagesData_->GetItem(id);

    if (!getItemResult) {
        promise.SetError(UCMErrorDomainErrc::kInvalidTransferId);
        return promise.get_future();
    }

    auto result = getItemResult->DeleteTransfer();

    if (result == DeleteTransferReturnType::kSuccess) {
        packagesData_->DeleteItem(id);
        promise.set_value();
    } else {
        promise.SetError(UCMErrorDomainErrc::kGeneralReject);
    }

    return promise.get_future();
}
void PackageManagerImpl::CommitActions()
{
    for (auto& iterator : executedActions_) {
        if (iterator) {
            iterator->CommitChanges();  // We intentionally ignore the errors here
        }
    }
    executedActions_.clear();  // this makes the PackageManagerImpl instance ready for the next loop.
}
void PackageManagerImpl::RevertActions()
{
    for (auto& iterator : executedActions_) {
        if (iterator) {
            iterator->RevertChanges();  // We intentionally ignore the errors here
        }
    }
    executedActions_.clear();  // this makes the PackageManagerImpl instance ready for the next loop.
}

void PackageManagerImpl::ResetSWCLChangeInfo()
{
    SWCLManager_->ResetSWCLChangeInfo();
}

int PackageManagerImpl::versionCompare(ara::core::String& v1, ara::core::String& v2)
{
    // vnum stores each numeric
    // part of version
    int vnum1 = 0, vnum2 = 0;
    ara::core::String::iterator it1 = v1.begin();
    ara::core::String::iterator it2 = v2.begin();
    // loop until both string are
    // processed
    for (it1 , it2; (it1 != v1.end()
                            || it2 != v2.end());) {
        // storing numeric part of
        // version 1 in vnum1
        while (it1 != v1.end() && *it1 != '.') {
            vnum1 = vnum1 * 10 + (*it1 - '0');
            it1++;
        }
 
        // storing numeric part of
        // version 2 in vnum2
        while (it2 != v2.end() && *it2 != '.') {
            vnum2 = vnum2 * 10 + (*it2 - '0');
            it2++;
        }
 
        if (vnum1 > vnum2)
            return 1;
        if (vnum2 > vnum1)
            return -1;
 
        // if equal, reset variables and
        // go for next numeric part
        vnum1 = vnum2 = 0;
        it1++;
        it2++;
    }
    return 0;
}

}  // namespace pkgmgr
}  // namespace ucm
}  // namespace ara
