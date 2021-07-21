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

#include <boost/filesystem/fstream.hpp>
#include "package_manager_state/package_manager_state.h"
#include "package_manager_impl/package_manager_impl.h"
#include "ara/core/promise.h"
namespace ara
{
namespace ucm
{
namespace pkgmgr
{


Future<void> PackageManagerState::OnProcessSwPackageSucceeded(PackageManager& pm, StateAccessor& accessor)
{
    (void)pm;
    (void)accessor;
    Promise<void> promise; 
    promise.SetError(UCMErrorDomainErrc::kOperationNotPermitted); //event 
    return promise.get_future();
}

Future<void> PackageManagerState::OnProcessSwPackageFailed(PackageManager& pm, StateAccessor& accessor)
{
    (void)pm;
    (void)accessor;
    Promise<void> promise; 
    promise.SetError(UCMErrorDomainErrc::kOperationNotPermitted); //event 
    return promise.get_future();
}
//completed method
Future<void> PackageManagerState::Activate(PackageManager& pm,StateAccessor& accessor,const ActivateOptionType& option)
{
    (void)pm;
    (void)accessor;
    (void)option;
    // we casting unused parameter to void to avoid the compiler error unused paramter 
    Promise<void> promise; //here we create a promise to check shared status (variable) or shared exaption 
    promise.SetError(UCMErrorDomainErrc::kErrorDuringActivation); //event 
    return promise.get_future();
}
//completed method 
Future<void> PackageManagerState::Cancel(const TransferIdType& id)
{
    (void)id;
    Promise<void> promise;
    promise.SetError(UCMErrorDomainErrc::kOperationNotPermitted);
    return promise.get_future();
}

void PackageManagerState::Done(StateAccessor& accessor)
{
    (void)accessor;
}

void PackageManagerState::OnSuccess(PackageManager& pm, StateAccessor& accessor)
{
    (void)pm;
    (void)accessor;
}

void PackageManagerState::OnFailure(StateAccessor& accessor)
{
    (void)accessor;
}

void PackageManagerState::SaveCurrentState(PackageManagerStatusType state)
{
    /*
    ara::core::String storgeStatePath = fs_.BuildPath("/home/root/","bin");
    std::fstream file;
       file.open ("state.dat", std::ios_base::out);
   if(file.is_open()){
      file << std::to_string(static_cast<std::uint8_t>(state)) <<std::endl;
      file.close();
   }else{
      std::cerr<<"Error opening file!!"<<std::endl;
   }*/
    // Save the current state.
    // Saving could be done directly here or through
    // an impl. obj. passed to this function through the state constructor.
    // TBD save mechanism (filesystem, persistency KVS).
    (void)state;
}

Future<void> PackageManagerState::Finish(PackageManager& pm, StateAccessor& accessor)
{
    (void)pm;
    (void)accessor;

    Promise<void> promise;
    promise.SetError(UCMErrorDomainErrc::kOperationNotPermitted);
    return promise.get_future();
}

Future<void> PackageManagerState::RevertProcessedSwPackages(PackageManager& pm, StateAccessor& accessor)
{
    (void)pm;
    (void)accessor;

    Promise<void> promise;
    promise.SetError(UCMErrorDomainErrc::kNotAbleToRevertPackages);
    return promise.get_future();
}

Future<void> PackageManagerState::Rollback(PackageManager& pm, StateAccessor& accessor)
{
    (void)pm;
    (void)accessor;

    Promise<void> promise;
    promise.SetError(UCMErrorDomainErrc::kNotAbleToRollback);
    return promise.get_future();
}

Future<void> PackageManagerState::ProcessSwPackage(PackageManager& pm,
    StateAccessor& accessor,
    const TransferIdType& id)
{
    (void)pm;
    (void)accessor;
    (void)id;

    Promise<void> promise;
    promise.SetError(UCMErrorDomainErrc::kOperationNotPermitted);
    return promise.get_future();
}

}  // namespace pkgmgr
}  // namespace ucm
}  // namespace ara
