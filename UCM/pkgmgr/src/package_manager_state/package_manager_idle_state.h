/*
#####################################################################################
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
#ifndef ARA_UCM_PKGMGR_PACKAGE_MANAGER_STATE_PACKAGE_MANAGER_IDLE_STATE_H_
#define ARA_UCM_PKGMGR_PACKAGE_MANAGER_STATE_PACKAGE_MANAGER_IDLE_STATE_H_


#include <memory>
#include "package_manager_state/package_manager_state.h"
#include "transfer/receive_types.h"

namespace ara
{
    namespace ucm
    {
        namespace pkgmgr
        {
            /// @brief Class representing Idle (default) state of Package Manager
            ///
            /// @uptrace{SWS_UCM_00080}
            class PackageManagerIdleState final : public PackageManagerState
            {
            public:
                /// @brief Creates a new instance of Idle state and saves the current state
                PackageManagerIdleState();
                ~PackageManagerIdleState() = default;
                /// <inheritdoc>
                PackageManagerStatusType GetStatus() const override
                {
                    return PackageManagerStatusType::kIdle;
                }
                /// <inheritdoc>
                virtual Future<void> ProcessSwPackage(PackageManager &pm,
                                                      StateAccessor &accessor,
                                                      const TransferIdType &id) override;
            



            };

        } // namespace pkgmgr
    }     // namespace ucm
} // namespace ara

#endif   // ARA_UCM_PKGMGR_PACKAGE_MANAGER_STATE_PACKAGE_MANAGER_IDLE_STATE_H_