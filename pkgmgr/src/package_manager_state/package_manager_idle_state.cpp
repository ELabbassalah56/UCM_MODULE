/*
#####################################################################################
#                                                                                   #
#                                                                                   #
#                                                                                   #
#                                                                                   #
#                          VALEO  GP ADAPTIVE AUTOSAR                               #
#                                                                                   #
#                                                                                   #
#                                                                                   #
#                                                                                   #
#                                                                                   #
#                                                               
#                                                                                   #
#                                                                                   #
#                                                                                   #
#                                                                                   #
#                                                                                   #
#                                                                                   #
#                                                                                   #
#                                                                                   #
#                                                                                   #
#                                                                                   #
#                                                                                   #
#                                                                                   #
#                                                                                   #
#                                                                                   #
#                                                                                   #
#                                                                                   #
#                                                                                   #
#                                                                                   #
#                                                                                   #
#                                                                                   #
#                                                                                   #
#                                                                                   #
#                                                                                   #
#                                                                                   #
#                                                                                   #
#                                                                                   #
#                                                                                   #
#                                                                                   #
#                                                                                   #
#                                                                                   #
#                                                                                   #
#                                                                                   #
#                                                                                   #
#                                                                                   #
#                                                                                   #
#                                                                                   #
#                                                                                   #
#                                                                                   #
#                                                                                   #
#                                                                                   #
#                                                                                   #
#                                                                                   #
#                                                                                   #
#                                                                                   #
#                                                                                   #
#                                                                                   #
#                                                                                   #
#                                                                                   #
#                                                                                   #
#                                                                                   #
#                                                                                   #
#                                                                                   #
#####################################################################################
*/
#include "package_manager_state/package_manager_idle_state.h"
#include "package_manager_state/package_manager_processing_state.h"
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

            PackageManagerIdleState::PackageManagerIdleState()
            {
                SaveCurrentState(this->GetStatus());
                log_.LogInfo() << "Package Manager Idle State";
            }

            Future<void> PackageManagerIdleState::ProcessSwPackage(PackageManager &pm,
                                                                   StateAccessor &accessor,
                                                                   const TransferIdType &id)
            {
                std::cout<<"\n\n\nProcessing in IDel State invoke \n\n\n"<<std::endl;

                Promise<void> promise;
                auto future = promise.get_future();

                InterruptToken token{false};

                    accessor.Reset(std::make_unique<PackageManagerProcessingState>(token, id, PackageManagerStatusType::kIdle));

                    InterruptibleThread th(
                        token,
                        [&accessor](ara::core::Promise<void> &&pr, std::reference_wrapper<PackageManager> p, TransferIdType const &i) {
                            static std::uint8_t countPkgProcessed = 0;
                            countPkgProcessed += 1; 
                             std::cout<<"\n\n\nProcessing Start\n\n\n"<<std::endl;

                            auto out = p.get().DoProcessSwPackage(i);
                            if (out.HasValue())
                            {
                                std::cout<<"\n\n\nProcessing Success Return Value\n\n\n"<<std::endl;
                                pr.set_value();
                            }
                            else if ((out.Error() == UCMErrorDomainErrc::kProcessSwPackageCancelled) && (countPkgProcessed == 1))
                            {
                                //[SWS_UCM_00149]
                                countPkgProcessed = 0;
                                std::cout<<"\n\n\nProcessing Faild Return Value and return to Idel state\n\n\n"<<std::endl;
                                accessor.Reset(std::make_unique<PackageManagerIdleState>());
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

        } // namespace pkgmgr
    }     // namespace ucm
} // namespace ara