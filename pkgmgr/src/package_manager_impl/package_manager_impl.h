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

#ifndef ARA_UCM_PKGMGR_PACKAGE_MANAGER_INTERNAL_PACKAGE_MANAGER_INTERNAL_IMPL_H_
#define ARA_UCM_PKGMGR_PACKAGE_MANAGER_INTERNAL_PACKAGE_MANAGER_INTERNAL_IMPL_H_

#include <memory>

#include "package_manager_state/package_manager_state.h"
#include "package_manager_state/package_manager_idle_state.h"
#include "transfer/streamable_software_package.h"
#include "transfer/streamable_software_package_factory.h"

#include "parsing/application.h"
#include "parsing/ucm_strings.h"
#include "parsing/software_package_parser.h"
#include "extraction/software_package_extractor.h"

#include "storage/swcl_manager.h"
#include "storage/processes_list_manager.h"

#include "synchronized_storage.h"
#include "serial_id_generator.h"

#include "ara/core/promise.h"
#include "ara/core/string.h"
#include "ara/log/logging.h"
#include "ara/ucm/pkgmgr/packagemanagement_common.h"


#include "package_manager/package_manager.h"

namespace ara
{
namespace ucm
{
namespace pkgmgr
{

using Skeleton = ara::ucm::pkgmgr::skeleton::PackageManagementSkeleton;
using GetHistoryOutput = Skeleton::GetHistoryOutput;

using GetSwPackagesOutput = PackageManagement::GetSwPackagesOutput;
using GetSwProcessProgressFuture = ara::core::Future<PackageManagement::GetSwProcessProgressOutput>;
using TransferStartFuture = ara::core::Future<PackageManagement::TransferStartOutput>;
using TransferDataFuture = ara::core::Future<void>;
using TransferExitFuture = ara::core::Future<void>;
using SwClusterInfoVectorType = ::ara::ucm::pkgmgr::SwClusterInfoVectorType;
using DeleteTransferReturnType = ::ara::ucm::pkgmgr::GeneralResponseType;


using ara::core::Result;
using ara::core::ErrorCode;

/// @brief This class is the entry point for package management tasks
/// such as installing, uninstalling and update of software clusters
class PackageManagerImpl
{
public:
    /// @brief Initializes the PackageManagerImpl
    ///
    /// @param softwarePackageExtractor The extractor defines how a compressed software package
    /// @param softwarePackageParser the parser for software packages
    /// can be extracted
    /// @param initialState First state of the manager
    /// @param SWCLManager Manager to deal with software packages
    /// @param packageDataFactory Factory of incoming packages
    /// @param installDir The directory where new applications should be installed
    PackageManagerImpl(std::unique_ptr<SoftwarePackageExtractor> softwarePackageExtractor,
        std::unique_ptr<SoftwarePackageParser> softwarePackageParser,
        std::unique_ptr<SWCLManager> SWCLManager,
        std::unique_ptr<ProcessesListManager> processesListManager,
        std::unique_ptr<StreamableSoftwarePackageFactory> packageDataFactory,
        std::unique_ptr<SynchronizedStorage<TransferIdType, StreamableSoftwarePackage>> packagesData,
        const core::String& installDir);
    virtual ~PackageManagerImpl();

    /// @brief Entry point for processing a sofware package. Processing by:
    /// first validating the content of the package,
    /// then checking various preconditions and finally calling the proper process routine,
    /// according to the specified type
    ///
    /// @param package The steamable software package with software package archive
    ///
    /// @return The result of of processing this sw package, which specifies if the
    /// application has been successfully installed.
    ///
    /// @uptrace{SWS_UCM_00001}
    Result<void, ErrorCode> ProcessSoftwarePackage(Streamable& package);

    /// @brief Block-wise transfer of a Software Package.
    ///
    /// @param id Transfer ID of the currently running request.
    /// @param data Data block of the Software Package.
    /// @param blockCounter Block counter of the current block.
    ///
    /// @return The result of transferring current data block, which specifies if the
    /// sw package has been successfully transferred.
    virtual TransferDataFuture TransferData(const TransferIdType& id,
        const ByteVectorType& data,
        const std::uint32_t& blockCounter);

    /// @brief Finish the transfer of a Software Package.
    ///
    /// @param id Transfer ID of the currently running request.
    ///
    /// @return The result of finishing this sw package transfer, which specifies if the
    /// sw package has been successfully transferred.
    virtual TransferExitFuture TransferExit(const TransferIdType& id);

    /// @brief Start the transfer of a Software Package.
    /// Transfer Id for subsequent calls to TransferData will be generated
    /// and returned as a part of TransferStartOutput.
    ///
    /// @param size Size (in bytes) of the Software Package to be transferred.
    ///
    /// @return The future that contains the result of the transfer start operation
    /// (transfer id generated for this operation or error code).
    virtual TransferStartFuture TransferStart(const std::uint32_t& size);

    /// @brief Retrieve a Software Package list.
    ///
    /// @return List of all Software Packages that have been successfully
    /// transferred and are ready to be installed.
    virtual GetSwPackagesOutput GetSwPackages() const;

    /// @brief Retrieve log messages for a particular Software Package.
    ///
    /// @param id Transfer ID to get log for.
    ///
    /// @return List of log entries for particular Transfer Id.
    virtual ara::core::Future<PackageManagement::GetLogOutput> GetLog(const TransferIdType id) const;

    /// @brief Retrieve a Software Clusters data (name, state, version)
    ///
    /// @return List of Software Clusters's data
    virtual SwClusterInfoVectorType GetSwClusterInfo() const;
    
    /// @brief Retrieve a UCM data (name,version,Release)
    ///
    /// @return List of UCM's data
    virtual UCMInfoVectorOfStringType UCMInfoService() const;
    
    /// @brief Gets the software clusters change info vector
    ///
    /// @returns vector of software clusters info
    virtual const SwClusterInfoVectorType& GetSwClusterChangeInfo() const;

    /// @brief Retrive the last saved state of the package manager.
    /// If none is found then a Ready state instance is returned
    ///
    /// @return Package manager state instance
    virtual std::unique_ptr<PackageManagerState> RetrieveState(void);

    /// @brief Actual unpacking of the file received by Transfer
    ///
    /// @returns Result of unpacking(=processing): in case of error returns its code, otherwise returns nothing.
    Result<void, ErrorCode> DoProcessSwPackage(const TransferIdType& id);

    /// @brief Updates the processes list with current existing applications
    void UpdateProcessesList();

    /// @brief Get the progress of the currently processed Software Package.
    ///
    /// @param id Transfer ID of Software Package.
    ///
    /// @return The progress of the current package processing (0x00 - 0x64).
    ///
    /// @uptrace{SWS_UCM_00018}
    GetSwProcessProgressFuture GetSwProcessProgress(const TransferIdType& id) const;

    /// @brief Resets the software clusters change info. vector
    ///
    /// @returns nothing
    void ResetSWCLChangeInfo();

    /// <inheritdoc>
    virtual ara::core::Future<void> DeleteTransfer(const TransferIdType& id);

    GetHistoryOutput GetHistory(const std::uint64_t& timestampGE, const std::uint64_t& timestampLT);

    /// @brief Internal trigger, which is provided for actions
    void CommitActions();

    /// @brief Internal trigger, which is provided for actions
    void RevertActions();

   

private:
    /// @brief Creates an absolute file path that is used as the target extraction directory,
    /// by using the name of the archive as the name of the extraction folder.
    ///
    /// @param fullFilePath The absolute file path to the archive
    ///
    /// @return An absolute file path to a folder that is located in the same directory as
    /// the archive and has the same name as the archive (without the archive extension)
    virtual core::String GetExtractionTargetDirectory(const core::String& fullFilePath) const;

    /// @brief Creates id for a new software package
    ///
    /// @returns Id for a new package, or error in case of unsufficient memory
    TransferStartReturnType GetNextID() const;

    /// @brief Implements extraction of the a software package
    std::unique_ptr<SoftwarePackageExtractor> softwarePackageExtractor_;

    /// @brief Encapsulates the parsing of a received archive into a software package object
    std::unique_ptr<SoftwarePackageParser> softwarePackageParser_;

    /// @brief Implements actions with software clusters
    std::unique_ptr<SWCLManager> SWCLManager_;

    /// @brief Implements actions with processes list
    std::unique_ptr<ProcessesListManager> processesListManager_;

    std::unique_ptr<StreamableSoftwarePackageFactory> softwarePackageFactory_;

    /// @brief Container with information about software packages at work
    std::unique_ptr<SynchronizedStorage<TransferIdType, StreamableSoftwarePackage>> packagesData_;

    /// @brief Provides transfer id generation
    SerialIDGenerator<TransferIdType> generator_;

    /// @brief The directory where applications are installed
    /// @note New applications will also be installed in this directory
    const core::String installDirectory_;

    /// @brief The list of all executed actions.
    ///
    /// The list of all executed actions is filled in PROCESSING and emptied in CLEANING_UP state.
    std::vector<std::unique_ptr<ReversibleAction>> executedActions_;

    /// @brief Logger for logging errors
    ara::log::Logger& log_{ara::log::CreateLogger("PKGM", "PackageManagerImpl context")};

    /// @brief pointer to  running  application in UCM  
    std::unique_ptr<Application> appInfo_;
    /// @brief pointer to new software cluster input to ucm 
    std::unique_ptr<SoftwareCluster> swClInfo_;
    /// @brief pointer to packagmangment to ucm
    std::unique_ptr<PackageManagement> pkgCommon_; 
    /// @brief Version of ucm Service 
    static constexpr constexpr_str service_version = "1.0.0_0";
    /// @brief pointer to ucm INFO
    std::unique_ptr<ucminfo> ucmInfo_ = std::make_unique<ucminfo>() ;

};
}  // namespace pkgmgr
}  // namespace ucm
}  // namespace ara

#endif  // ARA_UCM_PKGMGR_PACKAGE_MANAGER_INTERNAL_PACKAGE_MANAGER_INTERNAL_IMPL_H_
