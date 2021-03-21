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
#include "package_manager_state/package_manager_activating_state.h"
#include "package_manager_state/package_manager_processing_state.h"
#include "package_manager_state/package_manager_cleaningup_state.h"
#include "package_manager_impl/package_manager_impl.h"
#include "package_manager/package_manager.h"
#include "interruptible_thread/interrupt_token.h"
#include "interruptible_thread/interruptible_thread.h"

#include <memory>

namespace ara
{
    namespace ucm
    {
        namespace pkgmgr
        {

            PackageManagerReadyState::PackageManagerReadyState()
            {
                SaveCurrentState(this->GetStatus());

                log_.LogInfo() << "Package Manager Ready State";
            }

            Future<void> PackageManagerReadyState::ProcessSwPackage(PackageManager &pm,
                                                                    StateAccessor &accessor,
                                                                    const TransferIdType &id)
            {
                Promise<void> promise;
                auto future = promise.get_future();

                InterruptToken token{false};
                    accessor.Reset(std::make_unique<PackageManagerProcessingState>(token, id, PackageManagerStatusType::kReady));

                    InterruptibleThread th(
                        token,
                        [&accessor](ara::core::Promise<void> &&pr, std::reference_wrapper<PackageManager> p, TransferIdType const &i) {
                            static std::uint8_t countPkgProcessed = 0;
                            countPkgProcessed += 1;
                            auto out = p.get().DoProcessSwPackage(i);
                            if (out.HasValue())
                            {
                                pr.set_value();
                            }
                            else if ((out.Error() == UCMErrorDomainErrc::kProcessSwPackageCancelled) && (countPkgProcessed > 1))
                            {
                                //[SWS_UCM_00151]
                                countPkgProcessed = 0;
                                accessor.Reset(std::make_unique<PackageManagerReadyState>());
                                pr.SetError(out.Error());
                            }
                            else
                            {
                                pr.SetError(out.Error()); //someip set error
                            }
                        },
                        std::move(promise),
                        std::ref(pm),
                        id);

                    th.Detach();
                
                return future;
            }

            Future<void> PackageManagerReadyState::Activate(PackageManager &pm,
                                                            StateAccessor &accessor,
                                                            const ActivateOptionType &option)
            {
                accessor.Reset(std::make_unique<PackageManagerActivatingState>()); //Next state activating state
                Promise<void> promise;
                Future<void> future = promise.get_future();
                pm.StartActivation(std::move(promise), option);
                return future;
            }

            auto PackageManagerReadyState::RevertProcessedSwPackages(PackageManager &pm, StateAccessor &accessor)
                -> decltype(PackageManagerState::RevertProcessedSwPackages(pm, accessor))
            {
                /* here shall support a recovery
       mechanism in case of failed
       update process
    */
                accessor.Reset(std::make_unique<PackageManagerCleaningUpState>(pm, FinalActionType::Revert));
                Promise<void> promise;
                auto future = promise.get_future();
                pm.StartCleanup(std::move(promise));
                return future;
            }

        } // namespace pkgmgr
    }     // namespace ucm
} // namespace ara
