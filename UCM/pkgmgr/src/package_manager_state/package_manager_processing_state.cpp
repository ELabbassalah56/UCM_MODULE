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

#include "package_manager_state/package_manager_ready_state.h"
#include "package_manager_state/package_manager_idle_state.h"
#include "package_manager_state/package_manager_processing_state.h"
#include "package_manager_state/package_manager_cleaningup_state.h"
#include "package_manager/package_manager.h"

namespace ara
{
    namespace ucm
    {
        namespace pkgmgr
        {

            PackageManagerProcessingState::PackageManagerProcessingState(InterruptToken token,
                                                                         TransferIdType id,
                                                                         PackageManagerStatusType prevState)
                : PackageManagerState(), processedId_(id), token_(token), prevState_(prevState)
            {
                SaveCurrentState(this->GetStatus());
                log_.LogInfo() << "Package Manager Processing State";
            }
            // previous state paramater
            Future<void> PackageManagerProcessingState::OnProcessSwPackageSucceeded(PackageManager &pm, StateAccessor &accessor)
            {
                (void)pm;
                Promise<void> promise;
                std::cout << "Iam  here onProcessSwPackageSucceeded" << std::endl;
                accessor.Reset(std::make_unique<PackageManagerReadyState>());
                token_.Interrupt();
                promise.set_value();
                return promise.get_future();
            }

            Future<void> PackageManagerProcessingState::ProcessSwPackage(PackageManager &pm,
                                                                    StateAccessor &accessor,
                                                                    const TransferIdType &id)
            {
                (void)pm;
                (void)accessor;
                (void)id;

                Promise<void> promise;
                promise.SetError(UCMErrorDomainErrc::kServiceBusy);
                return promise.get_future();
            }
            Future<void> PackageManagerProcessingState::OnProcessSwPackageFailed(PackageManager &pm, StateAccessor &accessor)
            {
                (void)pm; // create variable prev

                Promise<void> promise;
                if (prevState_ == PackageManagerStatusType::kReady)
                {
                    accessor.Reset(std::make_unique<PackageManagerReadyState>());
                    promise.set_value();
                }
                else if (prevState_ == PackageManagerStatusType::kIdle)
                {
                    accessor.Reset(std::make_unique<PackageManagerIdleState>());
                    promise.set_value();
                }
                else
                {
                    promise.SetError(UCMErrorDomainErrc::kOperationNotPermitted);
                }
                return promise.get_future();
            }

            Future<void> PackageManagerProcessingState::Cancel(const TransferIdType &id)
            {
                Promise<void> promise;
                if (processedId_ == id)
                {

                    if (token_.Interrupt())
                    {
                        log_.LogInfo() << "Cancel of software package was successful";
                        promise.set_value();
                    }
                    else
                    {
                        log_.LogError() << "Cancel of software package ID =" << id
                                        << " failed:";
                        promise.SetError(UCMErrorDomainErrc::kCancelFailed);
                    }
                }
                else
                {
                    promise.SetError(UCMErrorDomainErrc::kInvalidTransferId);
                }
                return promise.get_future();
            }

            auto PackageManagerProcessingState::RevertProcessedSwPackages(PackageManager &pm, StateAccessor &accessor)
                -> decltype(PackageManagerState::RevertProcessedSwPackages(pm, accessor))
            {
                accessor.Reset(std::make_unique<PackageManagerCleaningUpState>(pm, FinalActionType::Revert));
                Promise<void> promise;
                auto future = promise.get_future();
                pm.StartCleanup(std::move(promise));
                return future;
            }

        } // namespace pkgmgr
    }     // namespace ucm
} // namespace ara
