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

/// @file
/// This file contains a dummy implementation of the service interface.
/// The method parameters are commented out because they are currently not used
/// which otherwise results in compiler warnings.

#include "package_manager/package_manager.h"
#include "package_manager_state/package_manager_state.h"
#include "interruptible_thread/interruptible_thread.h"
#include "package_manager_impl/package_manager_impl.h"
#include "parsing/application.h"

#include <chrono>
#include <thread>

namespace ara
{
    namespace ucm
    {
        namespace pkgmgr
        {

            using Skeleton = ara::ucm::pkgmgr::skeleton::PackageManagementSkeleton;
            using GetHistoryOutput = Skeleton::GetHistoryOutput;
            using GetSwClusterChangeInfoOutput = Skeleton::GetSwClusterChangeInfoOutput;
            using GetSwClusterInfoOutput = Skeleton::GetSwClusterInfoOutput;
            using GetSwPackagesOutput = Skeleton::GetSwPackagesOutput;
            using TransferStartOutput = Skeleton::TransferStartOutput;

            using ara::ucm::pkgmgr::ActivateOptionType;
            using ara::ucm::pkgmgr::ByteVectorType;
            using ara::ucm::pkgmgr::GeneralResponseType;
            using ara::ucm::pkgmgr::LogLevelType;
            using ara::ucm::pkgmgr::TransferIdType;

            //start//*@Abbas-Salah-gdb*/
            //intialize pkg manager constractor with instance id
            //End//*@Abbas-Salah-gdb*/
            PackageManager::PackageManager(std::unique_ptr<PackageManagerImpl> &&impl, com::InstanceIdentifier instance_id)
                : Skeleton(instance_id, ara::com::MethodCallProcessingMode::kEvent), impl_{std::move(impl)},
                  state_{impl_->RetrieveState(), [this](const PackageManagerState &s) { CurrentStatus.Update(s.GetStatus()); }}
            {
                CurrentStatus.Update(state_.Get()->GetStatus());
            }

            auto PackageManager::Activate(const ActivateOptionType &option) -> decltype(Skeleton::Activate(option))
            {

                auto accessor = state_.Get();
                return accessor->Activate(*this, accessor, option);
            }

            auto PackageManager::Cancel(const TransferIdType &id) -> decltype(Skeleton::Cancel(id))
            {

                auto accessor = state_.Get();
                return accessor->Cancel(id);
            }

            auto PackageManager::DeleteTransfer(const TransferIdType &id) -> decltype(Skeleton::DeleteTransfer(id))
            {
                return impl_->DeleteTransfer(id);
            }

            auto PackageManager::Finish() -> decltype(Skeleton::Finish())
            {
                auto accessor = state_.Get();
                return accessor->Finish(*this, accessor);
            }

            Future<GetHistoryOutput> PackageManager::GetHistory(const std::uint64_t &timestampGE, const std::uint64_t &timestampLT)
            {
                Promise<GetHistoryOutput> promise;
                promise.set_value(impl_->GetHistory(timestampGE, timestampLT));
                return promise.get_future();
            }

            auto PackageManager::GetLog(const TransferIdType &id) -> decltype(Skeleton::GetLog(id))
            {
                return impl_->GetLog(id);
            }

            Future<GetSwClusterChangeInfoOutput> PackageManager::GetSwClusterChangeInfo()
            {
                Promise<GetSwClusterChangeInfoOutput> promise;
                GetSwClusterChangeInfoOutput out;

                out.SwInfo = impl_->GetSwClusterChangeInfo();

                promise.set_value(out);
                return promise.get_future();
            }

            Future<GetSwClusterInfoOutput> PackageManager::GetSwClusterInfo()
            {
                Promise<GetSwClusterInfoOutput> promise;
                GetSwClusterInfoOutput out;

                out.SwInfo = impl_->GetSwClusterInfo();

                promise.set_value(out);
                return promise.get_future();
            }

            Future<GetSwPackagesOutput> PackageManager::GetSwPackages()
            {
                Promise<GetSwPackagesOutput> promise;
                promise.set_value(impl_->GetSwPackages());
                return promise.get_future();
            }

            auto PackageManager::GetSwProcessProgress(const TransferIdType &id) -> decltype(Skeleton::GetSwProcessProgress(id))
            {
                return impl_->GetSwProcessProgress(id);
            }

            auto PackageManager::ProcessSwPackage(const TransferIdType &id) -> decltype(Skeleton::ProcessSwPackage(id))
            {
                std::cout<<"\n\n\nProcessing State Start\n\n\n"<<std::endl;;
                auto accessor = state_.Get();
                return accessor->ProcessSwPackage(*this, accessor, id);
            }

            void PackageManager::StartCleanup(Promise<void> promise)
            {
                std::thread t(
                    [&](Promise<void> lambdaPromise) {
                        // inform the state that the action is complete
                        auto accessor = state_.Get();
                        accessor->Done(accessor);
                        lambdaPromise.set_value();
                    },
                    std::move(promise));
                t.detach();
            }

            void PackageManager::StartRollback(Promise<void> promise)
            {
                std::thread t(
                    [&](Promise<void> lambdaPromise) {
                        // imitate the long process   // rolling back proccessing  TODO
                        std::this_thread::sleep_for(std::chrono::seconds(1));
                        // inform the state that the action is complete
                        auto accessor = state_.Get();
                        accessor->Done(accessor);
                        lambdaPromise.set_value();
                    },
                    std::move(promise));
                t.detach();
            }

            void PackageManager::StartActivation(Promise<void> promise, const ActivateOptionType &option)
            {
                (void)option; // cancled this arguments in this release

                auto fun = [&](Promise<void> lambdaPromise) {
                    
                 //  check dependency in State KAdded ,KUpdate ,KPresent
                 
                    for (const auto &indexSwclChIn : impl_->GetSwClusterChangeInfo())
                    {


                            std::cout<<indexSwclChIn.Name<<std::endl; 
                            std::cout<<static_cast<uint8_t>(indexSwclChIn.State)<<std::endl; 
                            std::cout<<indexSwclChIn.Version<<std::endl; 

                        // for (const auto &indexSwclInfo : impl_->GetSwClusterInfo())
                        // {


                        //     // if (indexSwclChIn.State == SwClusterStateType::kAdded || indexSwclChIn.State == SwClusterStateType::kPresent || indexSwclChIn.State == SwClusterStateType::kUpdated)
                        //     // {
                                
                                
                        //     // }
                        //     // else
                        //     // {

                        //     // }
                        // }
                    }
                     
                    std::this_thread::sleep_for(std::chrono::seconds(1));
                    // inform the state that the action is complete
                    auto accessor = state_.Get();
                    accessor->OnSuccess(*this, accessor);
                    lambdaPromise.set_value();
                };

                std::thread t(fun,
                              std::move(promise));
                t.detach();
            }

            void PackageManager::StartVerification()
            {
                std::thread t([&]() {
                    // imitate the long process
                    std::this_thread::sleep_for(std::chrono::seconds(1));
                    // inform the state that the action is complete
                    auto accessor = state_.Get();
                    accessor->OnSuccess(*this, accessor);
                });
                t.detach();
            }

            Result<void> PackageManager::DoProcessSwPackage(const TransferIdType &id)
            {
                auto accessor = state_.Get();

                if (InterruptibleThread::IsInterrupted())
                {
                    // interrupted before the start, no cleanup is needed
                    //SWS_UCM_00149
                    return Result<void>::FromError(UCMErrorDomainErrc::kProcessSwPackageCancelled);
                }
                else
                {
                    auto processingResult = impl_->DoProcessSwPackage(id);
                    if (processingResult.HasValue())
                    {
                        //SWS_UCM_00083
                        accessor->OnProcessSwPackageSucceeded(*this, accessor);
                    }
                    else
                    {
                        accessor->OnProcessSwPackageFailed(*this, accessor);
                    }
                    return processingResult;
                }
            }

            auto PackageManager::RevertProcessedSwPackages() -> decltype(Skeleton::RevertProcessedSwPackages())
            {
                auto accessor = state_.Get();
                return accessor->RevertProcessedSwPackages(*this, accessor);
            }

            auto PackageManager::Rollback() -> decltype(Skeleton::Rollback())
            {
                auto accessor = state_.Get();
                return accessor->Rollback(*this, accessor);
            }

            auto PackageManager::SetLogLevel(const TransferIdType &id, const LogLevelType &logLevel)
                -> decltype(Skeleton::SetLogLevel(id, logLevel))
            {
                Promise<void> promise;
                (void)id;
                (void)logLevel;
                promise.set_value();
                return promise.get_future();
            }

            auto PackageManager::TransferData(const TransferIdType &id,
                                              const ByteVectorType &data,
                                              const std::uint32_t &blockCounter) -> decltype(Skeleton::TransferData(id, data, blockCounter))
            {
                return impl_->TransferData(id, data, blockCounter);
            }

            auto PackageManager::TransferExit(const TransferIdType &id) -> decltype(Skeleton::TransferExit(id))
            {
                return impl_->TransferExit(id);
            }

            auto PackageManager::TransferStart(const std::uint64_t &size) -> decltype(Skeleton::TransferStart(size))
            {
                return impl_->TransferStart(size);
            }
            void PackageManager::CommitActions()
            {
                // Transfer the control to the package manager implementation.
                impl_->CommitActions();
            }
            void PackageManager::RevertActions()
            {
                // Transfer the control to the package manager implementation.
                impl_->RevertActions();
            }

            void PackageManager::ResetSwclChangeInfo()
            {
                impl_->ResetSWCLChangeInfo();
            }

        } // namespace pkgmgr
    }     // namespace ucm
} // namespace ara
