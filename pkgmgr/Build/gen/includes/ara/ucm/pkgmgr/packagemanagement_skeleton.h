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

#ifndef _home_user_Desktop_GP_VALEO_AP_ara_project_R19_11_ara_api_ucm_pkgmgr_Build_gen_includes_ara_ucm_pkgmgr_packagemanagement_skeleton_h
#define _home_user_Desktop_GP_VALEO_AP_ara_project_R19_11_ara_api_ucm_pkgmgr_Build_gen_includes_ara_ucm_pkgmgr_packagemanagement_skeleton_h

#include "ara/com/internal/skeleton/ara_skeleton_base.h"

#include "ara/com/illegal_state_exception.h"
#include "packagemanagement_common.h"

namespace ara {
namespace ucm {
namespace pkgmgr {
namespace skeleton {

namespace fields {
  /// @uptrace{SWS_CM_00007}
  using CurrentStatus = ara::com::internal::skeleton::FieldDispatcher<::ara::ucm::pkgmgr::PackageManagerStatusType>;
} // namespace fields


/// @uptrace{SWS_CM_00002}
class PackageManagementSkeleton : public ara::ucm::pkgmgr::PackageManagement, public ara::com::internal::skeleton::TypedServiceImplBase<PackageManagementSkeleton> {
public:
  /// @uptrace{SWS_CM_00130}
  PackageManagementSkeleton(ara::com::InstanceIdentifier instance_id, ara::com::MethodCallProcessingMode mode = ara::com::MethodCallProcessingMode::kEvent) : ara::com::internal::skeleton::TypedServiceImplBase<PackageManagementSkeleton>(instance_id, mode) {}
  virtual ~PackageManagementSkeleton() {
    StopOfferService();
  }

  /// @brief Skeleton shall be move constructable.
  ///
  /// @uptrace{SWS_CM_00135}
  explicit PackageManagementSkeleton(PackageManagementSkeleton&&) = default;

  /// @brief Skeleton shall be move assignable.
  ///
  /// @uptrace{SWS_CM_00135}
  PackageManagementSkeleton& operator=(PackageManagementSkeleton&&) = default;

  /// @brief Skeleton shall not be copy constructable.
  ///
  /// @uptrace{SWS_CM_00134}
  explicit PackageManagementSkeleton(const PackageManagementSkeleton&) = delete;

  /// @brief Skeleton shall not be copy assignable.
  ///
  /// @uptrace{SWS_CM_00134}
  PackageManagementSkeleton& operator=(const PackageManagementSkeleton&) = delete;

  void OfferService() {
    if(!CurrentStatus.IsInitialized()) {
      throw ara::com::IllegalStateException("Attempt to offer service \"/ara/ucm/pkgmgr/portinterfaces/PackageManagement\" with uninitialized field \"CurrentStatus\"");
    }

    ara::com::internal::skeleton::TypedServiceImplBase<PackageManagementSkeleton>::OfferService();
  }

  /// @uptrace{SWS_CM_00191}
  virtual ara::core::Future<void> Activate(const ::ara::ucm::pkgmgr::ActivateOptionType& option) = 0;
  virtual ara::core::Future<void> Cancel(const ::ara::ucm::pkgmgr::TransferIdType& id) = 0;
  virtual ara::core::Future<void> DeleteTransfer(const ::ara::ucm::pkgmgr::TransferIdType& id) = 0;
  virtual ara::core::Future<void> Finish() = 0;
  using ara::ucm::pkgmgr::PackageManagement::GetHistoryOutput;
  virtual ara::core::Future<GetHistoryOutput> GetHistory(const std::uint64_t& timestampGE,const std::uint64_t& timestampLT) = 0;
  using ara::ucm::pkgmgr::PackageManagement::GetLogOutput;
  virtual ara::core::Future<GetLogOutput> GetLog(const ::ara::ucm::pkgmgr::TransferIdType& id) = 0;
  using ara::ucm::pkgmgr::PackageManagement::GetSwClusterChangeInfoOutput;
  virtual ara::core::Future<GetSwClusterChangeInfoOutput> GetSwClusterChangeInfo() = 0;
  using ara::ucm::pkgmgr::PackageManagement::GetSwClusterInfoOutput;
  virtual ara::core::Future<GetSwClusterInfoOutput> GetSwClusterInfo() = 0;
  using ara::ucm::pkgmgr::PackageManagement::GetSwPackagesOutput;
  virtual ara::core::Future<GetSwPackagesOutput> GetSwPackages() = 0;
  using ara::ucm::pkgmgr::PackageManagement::GetSwProcessProgressOutput;
  virtual ara::core::Future<GetSwProcessProgressOutput> GetSwProcessProgress(const ::ara::ucm::pkgmgr::TransferIdType& id) = 0;
  virtual ara::core::Future<void> ProcessSwPackage(const ::ara::ucm::pkgmgr::TransferIdType& id) = 0;
  virtual ara::core::Future<void> RevertProcessedSwPackages() = 0;
  virtual ara::core::Future<void> Rollback() = 0;
  virtual ara::core::Future<void> SetLogLevel(const ::ara::ucm::pkgmgr::TransferIdType& id,const ::ara::ucm::pkgmgr::LogLevelType& logLevel) = 0;
  virtual ara::core::Future<void> TransferData(const ::ara::ucm::pkgmgr::TransferIdType& id,const ::ara::ucm::pkgmgr::ByteVectorType& data,const std::uint32_t& blockCounter) = 0;
  virtual ara::core::Future<void> TransferExit(const ::ara::ucm::pkgmgr::TransferIdType& id) = 0;
  using ara::ucm::pkgmgr::PackageManagement::TransferStartOutput;
  virtual ara::core::Future<TransferStartOutput> TransferStart(const std::uint64_t& size) = 0;
  fields::CurrentStatus CurrentStatus;
};

} // namespace skeleton
} // namespace pkgmgr
} // namespace ucm
} // namespace ara

#endif // _home_user_Desktop_GP_VALEO_AP_ara_project_R19_11_ara_api_ucm_pkgmgr_Build_gen_includes_ara_ucm_pkgmgr_packagemanagement_skeleton_h

