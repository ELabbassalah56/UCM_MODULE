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

#ifndef ARA_UCM_PKGMGR_PACKAGE_MANAGER_PACKAGE_MANAGER_H_
#define ARA_UCM_PKGMGR_PACKAGE_MANAGER_PACKAGE_MANAGER_H_

#include <utility>

#include "locked_ptr_wrapper.h"
#include "ara/log/logging.h"

#include "ara/ucm/pkgmgr/packagemanagement_skeleton.h"

namespace ara
{
namespace ucm
{
namespace pkgmgr
{

class PackageManagerState;
class PackageManagerImpl;

using ara::core::Future;
using ara::core::Promise;

using ara::core::Result;
using ara::core::ErrorCode;

/// @brief This is the implementation of the PackageManagement service interface
/// which can be used by clients to install, update or uninstall applications,
/// configuration data, calibration data or manifests.
/// commuincate with outworld as a provide service  ///*@Abbas-Salah-gdb*/
class PackageManager : public ara::ucm::pkgmgr::skeleton::PackageManagementSkeleton
{
    using Skeleton = ara::ucm::pkgmgr::skeleton::PackageManagementSkeleton;

public:
    /// @brief Initializes a new instance of this service with the given instance id
    ///
    /// @param impl smart pointer to implementation class object
    /// @param instance_id Instance identifier required by ara com
    explicit PackageManager(std::unique_ptr<PackageManagerImpl>&& impl, ara::com::InstanceIdentifier instance_id);  // move semintec 
              
    /// @brief Clean up the temporary files    
    ///
    /// @param promise to set the value / synchronize
    ///
    /// @uptrace{SWS_UCM_00158}
    void StartCleanup(Promise<void> promise);

    /// @brief Perform the rollback
    ///
    /// @uptrace{SWS_UCM_00005}
    void StartRollback(Promise<void> promise);

    /// @brief Perform the activation
    ///
    /// @param promise to set the value / synchronize
    /// @param option for the activation
    /// @uptrace{SWS_UCM_00153}
    void StartActivation(Promise<void> promise, const ActivateOptionType& option);

    /// @brief Perform the activation
    ///
    /// @uptrace{SWS_UCM_00154}
    void StartVerification();

    /// @brief Internal trigger, which is provided for actions
    void CommitActions();

    /// @brief Internal trigger, which is provided for actions
    void RevertActions();

    /// @brief Activate the processed components.
    ///
    /// @param option for activation
    /// @return The result of the activate operation, which specifies if the
    /// operation was successful.
    ///
    /// @uptrace{SWS_UCM_00025}
    /// @uptrace{SWS_UCM_00099}
    /// @uptrace{SWS_UCM_00100}
    /// @uptrace{SWS_UCM_00101}
    //start / /*@Abbas-Salah-gdb*/ 
    /// decltype it is just indector to the return type of function 
    /// when return type depend on type auto or template
    //end / /*@Abbas-Salah-gdb*/ 
    auto Activate(const ActivateOptionType& option = ActivateOptionType::kDefault) -> decltype(Skeleton::Activate(option)) override;

    /// @brief Abort an ongoing processing of a Software Package.
    ///
    /// @param id Transfer ID of Software Package.
    ///
    /// @return The result of cancelling operation, which specifies if the
    /// it was successful.
    ///
    /// @uptrace{SWS_UCM_00003}
    auto Cancel(const TransferIdType& id) -> decltype(Skeleton::Cancel(id)) override;

    /// @brief Delete a transferred Software Package.
    ///
    /// @param id Transfer ID of Software Package.
    ///
    /// @return The result of operation, which specifies if the
    /// request was successful.
    ///
    /// @uptrace{SWS_UCM_00021}
    auto DeleteTransfer(const TransferIdType& id) -> decltype(Skeleton::DeleteTransfer(id)) override;

    /// @brief Finish the processing for the current set of
    /// processed Software Packages. Cleanup all data of the processing
    /// including the sources of the Software Packages.
    ///
    /// @return The result of the finish operation, which specifies if the
    /// operation was successful.
    ///
    /// @uptrace{SWS_UCM_00020}
    auto Finish() -> decltype(Skeleton::Finish()) override;

    /// @brief Retrieve all actions performed by UCM in provided time range.
    ///
    /// @param timestampGE  begin (left side) of the time window, inclusive (Greater-or-Equal)
    /// @param timestampLT  end (right side) of the time window, exclusive (Less-Than)
    ///
    /// @returns the list of all actions performed by UCM in provided time range
    ///
    /// @uptrace{SWS_UCM_00115}
    Future<GetHistoryOutput> GetHistory(const std::uint64_t& timestampGE, const std::uint64_t& timestampLT) override;

    /// @brief Retrieve all log messages that have been stored
    /// for specific Transfer Id.
    ///
    /// @param id Transfer ID of Software Package.
    ///
    /// @return The list of Log Entries that have been stored for
    /// specific Transfer Id.
    ///
    /// @uptrace{SWS_UCM_00012}
    auto GetLog(const TransferIdType& id) -> decltype(Skeleton::GetLog(id)) override;

    /// @brief Retrieve a list of SoftwareClusters that have pending changes.
    /// The returned list includes all SoftwareClusters that are to be added,
    /// updated or removed. The list of changes is extended
    /// in the course of processing software Packages.
    ///
    /// @return List of SoftwareClusters that are in state kAdded,
    /// kUpdated or kRemoved.
    ///
    /// @uptrace{SWS_UCM_00030}
    Future<GetSwClusterChangeInfoOutput> GetSwClusterChangeInfo() override;

    /// @brief Retrieve a list of SoftwareClusters.
    ///
    /// @return List of installed SoftwareClusters that are in state kPresent.
    ///
    /// @uptrace{SWS_UCM_00004}
    Future<GetSwClusterInfoOutput> GetSwClusterInfo() override;

    /// @brief Retrieve a Software Package list.
    ///
    /// @return List of all Software Packages that have been successfully
    /// transferred and are ready to be installed.
    ///
    /// @uptrace{SWS_UCM_00069}
    Future<GetSwPackagesOutput> GetSwPackages() override;

    /// @brief Get the progress of the currently processed Software Package.
    ///
    /// @param id Transfer ID of Software Package.
    ///
    /// @return The progress of the current package processing (0x00 - 0x64).
    ///
    /// @uptrace{SWS_UCM_00018}
    auto GetSwProcessProgress(const TransferIdType& id) -> decltype(Skeleton::GetSwProcessProgress(id)) override;

    /// @brief Process transferred Software Package.
    ///
    /// @param id Transfer ID of Software Package.
    ///
    /// @return The result of processing, which specifies if the
    /// operation was successful.
    ///
    /// @uptrace{SWS_UCM_00001}
    Future<void> ProcessSwPackage(const TransferIdType& id) override;

    /// @brief Revert the changes done by processing (ProcessSwPackage) of one
    /// or several software packages.
    ///
    /// @return The result of the revert operation, which specifies if the
    /// operation was successful.
    ///
    /// @uptrace{SWS_UCM_00024}
    auto RevertProcessedSwPackages() -> decltype(Skeleton::RevertProcessedSwPackages()) override;

    /// @brief Rollback the system to the state
    /// before the packages were processed.
    ///
    /// @return The result of the rollback operation, which specifies if the
    /// operation was successful.
    ///
    /// @uptrace{SWS_UCM_00005}
    auto Rollback() -> decltype(Skeleton::Rollback()) override;

    /// @brief Setting the log level for the internal logging
    /// of the update process.
    ///
    /// @param id Transfer ID of the currently running request.
    /// @param logLevel The new log level to be used.
    ///
    /// @uptrace{SWS_UCM_00143}
    auto SetLogLevel(const TransferIdType& id, const LogLevelType& logLevel) -> decltype(Skeleton::SetLogLevel(id, logLevel)) override;

    /// @brief Block-wise transfer of a Software Package.
    ///
    /// @param id Transfer ID of the currently running request.
    /// @param data Data block of the Software Package.
    /// @param blockCounter Block counter of the current block.
    ///
    /// @return The result of transferring current data block, which specifies if the
    /// sw package has been successfully transferred.
    ///
    /// @uptrace{SWS_UCM_00007}
    /// @uptrace{SWS_UCM_00008}
    auto TransferData(const TransferIdType& id, const ByteVectorType& data, const std::uint32_t& blockCounter)
        -> decltype(Skeleton::TransferData(id, data, blockCounter)) override;

    /// @brief Finish the transfer of a Software Package.
    ///
    /// @param id Transfer ID of the currently running request.
    ///
    /// @return The result of finishing this sw package transfer, which specifies if the
    /// sw package has been successfully transferred.
    ///
    /// @uptrace{SWS_UCM_00007}
    /// @uptrace{SWS_UCM_00010}
    auto TransferExit(const TransferIdType& id) -> decltype(Skeleton::TransferExit(id)) override;

    /// @brief Start the transfer of a Software Package.
    /// Transfer Id for subsequent calls to TransferData will be generated
    /// and returned as a part of TransferStartOutput.
    ///
    /// @param size Size (in bytes) of the Software Package to be transferred.
    ///
    /// @return The struct which contains the result of the transfer start operation
    /// and transfer id generated for this operation.
    ///
    /// @uptrace{SWS_UCM_00007}
    /// @uptrace{SWS_UCM_00088}
    auto TransferStart(const std::uint64_t& size) -> decltype(Skeleton::TransferStart(size)) override;

    Result<void, ErrorCode> DoProcessSwPackage(const TransferIdType& id);

    /// @brief Resets the software clusters change info. vector
    ///
    /// @returns nothing
    void ResetSwclChangeInfo();
private:
     /// @brief Logger for logging errors  by abbas 
    ara::log::Logger& log_{ara::log::CreateLogger("PKGM", "PackageManagerImpl context")};
    /// @brief Smart pointer to the implementation class
    std::unique_ptr<PackageManagerImpl> impl_;

    /// @brief Pointer wrapper for package manager state
    LockedPtrWrapper<PackageManagerState> state_;
};

}  // namespace pkgmgr
}  // namespace ucm
}  // namespace ara

#endif  // ARA_UCM_PKGMGR_PACKAGE_MANAGER_PACKAGE_MANAGER_H_
