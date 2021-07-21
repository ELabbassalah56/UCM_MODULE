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

#ifndef ARA_UCM_PKGMGR_PACKAGE_MANAGER_STATE_PACKAGE_MANAGER_BUSY_STATE_H_
#define ARA_UCM_PKGMGR_PACKAGE_MANAGER_STATE_PACKAGE_MANAGER_BUSY_STATE_H_

#include "package_manager_state/package_manager_state.h"
#include "interruptible_thread/interrupt_token.h"

namespace ara
{
    namespace ucm
    {
        namespace pkgmgr
        {

            /// @brief Class representing Idle (default) state of Package Manager
            /// It is designed according to the State Pattern
            ///
            /// @uptrace{SWS_UCM_00007}
            class PackageManagerProcessingState final : public PackageManagerState
            {
            public:
                /// @brief Creates a new instance of Processing state with given id
                /// and interrupt token and saves the currentstate.
                ///
                ///
                /// @param token token for cancel implementation
                /// @param id currently processed package id
                PackageManagerProcessingState(InterruptToken token, TransferIdType id,PackageManagerStatusType prevState = PackageManagerStatusType::kIdle);
                ~PackageManagerProcessingState() = default;

                /// <inheritdoc>
                PackageManagerStatusType GetStatus() const override
                {
                    //[SWS_UCM_00081]
                    return PackageManagerStatusType::kProcessing;
                }
                //SWS_UCM_00017
                Future<void> ProcessSwPackage(PackageManager &pm,
                                              StateAccessor &accessor,
                                              const TransferIdType &id) override;
                /// <inheritdoc> >>>>>>>> processing
                Future<void> Cancel(const TransferIdType &id) override;

                /// <inheritdoc>
                auto RevertProcessedSwPackages(PackageManager &pm, StateAccessor &accessor)
                    -> decltype(PackageManagerState::RevertProcessedSwPackages(pm, accessor)) override;

                /// <inheritdoc>
                Future<void> OnProcessSwPackageSucceeded(PackageManager &pm, StateAccessor &accessor) override;

                /// <inheritdoc>
                Future<void> OnProcessSwPackageFailed(PackageManager &pm, StateAccessor &accessor) override;

            private:
                /// @brief The identifier of currently processed package
                TransferIdType processedId_;

                /// @brief A token for a thread interruption (thread is used for package processing)
                InterruptToken token_;

                PackageManagerStatusType prevState_;
            };

        } // namespace pkgmgr
    }     // namespace ucm
} // namespace ara

#endif // ARA_UCM_PKGMGR_PACKAGE_MANAGER_STATE_PACKAGE_MANAGER_BUSY_STATE_H_
