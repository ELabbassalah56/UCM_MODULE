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

#ifndef ARA_UCM_PKGMGR_PACKAGE_MANAGER_STATE_PACKAGE_MANAGER_STATE_H_
#define ARA_UCM_PKGMGR_PACKAGE_MANAGER_STATE_PACKAGE_MANAGER_STATE_H_

#include <memory>
#include "ara/core/string.h"
#include "ara/core/vector.h"
#include "ara/core/result.h"
#include "transfer/receive_types.h"
#include "ara/ucm/pkgmgr/packagemanagement_common.h"
#include "ara/ucm/pkgmgr/packagemanagement_skeleton.h"
#include "ara/core/future.h"
#include "locked_ptr_wrapper.h"
#include "ara/log/logging.h"
#include "filesystem.h"
#include "ara/per/key_value_storage.h"

namespace ara
{
namespace ucm
{
namespace pkgmgr
{

class PackageManager;
class PackageManagerState;
//@elabbas.salah comment
// Acessor meaning to setter state 
using StateAccessor = LockedPtrWrapper<PackageManagerState>::Accessor;

using ara::core::Future;

/// @brief This class is the base class for all classes
/// that represent Package Manager States
/// such as Ready, Processing and so on
class PackageManagerState
{
    using Skeleton = ara::ucm::pkgmgr::skeleton::PackageManagementSkeleton;

public:
    virtual ~PackageManagerState() = default;

    /// @brief Retrieve the Status which corresponds to the state.
    ///  PackageManagerStatusType jusit enumarate class define the pkgmanger state from 0x00U to 0x08U
    /// @return The corresponding status.
    ///
    virtual PackageManagerStatusType GetStatus() const = 0;  //complete method 

    /// @brief Activate the processed components.
    ///
    /// @param pm Package Manager object (used to perform action if needeed)  Action
    /// @param accessor State accessor object (used to change the current state if needed) Next State
    /// @param option Activate option (specifies the type of activation) type of  activation
    ///
    /// @return The result of the activate operation, which specifies if the
    /// operation was successful.
    ///
    /// @uptrace{SWS_UCM_00025} //partially implemented  Activation of SoftwareClusters
    /// @uptrace{SWS_UCM_00099} //Update of Adaptive Application( Adaptive application )  
    /// @uptrace{SWS_UCM_00100} //Update of Functional Clusters (fundamental function cluster)
    /// @uptrace{SWS_UCM_00101} //Update of Host (OS) 
    virtual Future<void> Activate(PackageManager& pm, StateAccessor& accessor, const ActivateOptionType& option = ActivateOptionType::kDefault);

    /// @brief Abort an ongoing processing of a Software Package.
    ///
    /// @param pm Package Manager object (used to perform action if needeed)
    /// @param accessor State accessor object (used to change the current state if needed)
    /// @param id Transfer ID of Software Package.
    ///
    /// @return The result of cancelling operation, which specifies if the
    /// it was successful.
    ///
    /// @uptrace{SWS_UCM_00003}
    virtual Future<void> Cancel(const TransferIdType& id) ;

    /// @brief Finish the processing for the current set of
    /// processed Software Packages. Cleanup all data of the processing
    /// including the sources of the Software Packages.
    ///
    /// @param pm Package Manager object (used to perform action if needeed)
    /// @param accessor State accessor object (used to change the current state if needed)
    ///
    /// @return The result of the finish operation, which specifies if the
    /// operation was successful.
    ///
    /// @uptrace{SWS_UCM_00020}
    virtual Future<void> Finish(PackageManager& pm, StateAccessor& accessor);

    /// Internal message
    virtual void Done(StateAccessor& accessor);

    /// Internal message
    virtual void OnSuccess(PackageManager& pm, StateAccessor& accessor);

    /// Internal message
    virtual void OnFailure(StateAccessor& accessor);

    /// @brief Revert the changes done by processing (ProcessSwPackage) of one
    /// or several software packages.
    ///
    /// @param pm Package Manager object (used to perform action if needeed)
    /// @param accessor State accessor object (used to change the current state if needed)
    ///
    /// @return The result of the revert operation, which specifies if the
    /// operation was successful.
    ///
    /// @uptrace{SWS_UCM_00024}
    virtual Future<void> RevertProcessedSwPackages(PackageManager& pm, StateAccessor& accessor);

    /// @brief Rollback the system to the state
    /// before the packages were processed.
    ///
    /// @param pm Package Manager object (used to perform action if needeed)
    /// @param accessor State accessor object (used to change the current state if needed)
    ///
    /// @return The result of the rollback operation, which specifies if the
    /// operation was successful.
    ///
    /// @uptrace{SWS_UCM_00005}
    virtual Future<void> Rollback(PackageManager& pm, StateAccessor& accessor);

    /// @brief Process transferred Software Package.
    ///
    /// @param pm Package Manager object (used to perform action if needeed)
    /// @param accessor State accessor object (used to change the current state if needed)
    /// @param id Transfer ID of Software Package.
    ///
    /// @return The result of processing, which specifies if the
    /// operation was successful.
    ///
    /// @uptrace{SWS_UCM_00001}
    virtual Future<void> ProcessSwPackage(PackageManager& pm, StateAccessor& accessor, const TransferIdType& id);

    /// @brief Handles successful process of the transferred software package.
    ///
    /// @param pm Package Manager object (used to perform action if needeed)
    /// @param accessor State accessor object (used to change the current state if needed)
    virtual Future<void> OnProcessSwPackageSucceeded(PackageManager& pm, StateAccessor& accessor);

    /// @brief Handles failed process of the transferred software package.
    ///
    /// @param pm Package Manager object (used to perform action if needeed)
    /// @param accessor State accessor object (used to change the current state if needed)
    virtual Future<void> OnProcessSwPackageFailed(PackageManager& pm, StateAccessor& accessor);

    /// @brief Saves the current status of the package manager.
    /// Is meant to be used to recover the state in case of the restart.
    ///
    /// @param state PackageManager state
    ///
    /// @uptrace{SWS_UCM_00157}
    /// @uptrace{SWS_UCM_00158}
    /// @uptrace{RS_UCM_00027}
    void SaveCurrentState(PackageManagerStatusType state);

protected:
    /// @brief Logger for logging current state
    ara::log::Logger& log_{ara::log::CreateLogger("PKST", "PackageManagerState context")};
   
    
};

}  // namespace pkgmgr
}  // namespace ucm
}  // namespace ara

#endif  // ARA_UCM_PKGMGR_PACKAGE_MANAGER_STATE_PACKAGE_MANAGER_STATE_H_
