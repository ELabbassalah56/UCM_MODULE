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

#ifndef _home_user_Desktop_GP_VALEO_AP_ara_project_R19_11_ara_api_ucm_pkgmgr_Build_gen_includes_ara_ucm_pkgmgr_packagemanagement_common_h
#define _home_user_Desktop_GP_VALEO_AP_ara_project_R19_11_ara_api_ucm_pkgmgr_Build_gen_includes_ara_ucm_pkgmgr_packagemanagement_common_h

#include <string.h>
#include "ara/core/array.h"

#include "ara/com/types.h"

#include "ara/com/exception.h"
#include "ara/core/error_code.h"
#include "ara/ucm/pkgmgr/impl_type_activateoptiontype.h"
#include "ara/ucm/pkgmgr/impl_type_bytevectortype.h"
#include "ara/ucm/pkgmgr/impl_type_gethistoryvectortype.h"
#include "ara/ucm/pkgmgr/impl_type_logleveltype.h"
#include "ara/ucm/pkgmgr/impl_type_logvectortype.h"
#include "ara/ucm/pkgmgr/impl_type_packagemanagerstatustype.h"
#include "ara/ucm/pkgmgr/impl_type_swclusterinfovectortype.h"
#include "ara/ucm/pkgmgr/impl_type_swpackageinfovectortype.h"
#include "ara/ucm/pkgmgr/impl_type_transferidtype.h"
#include <cstdint>
#include "error_domain_ucmerrordomain.h"
namespace ara {
namespace ucm {
namespace pkgmgr {

class PackageManagement {
 public:
  static constexpr ara::com::internal::ServiceId service_id = 0x3380;
  static constexpr ara::com::internal::ServiceVersion service_version = 0x01000000;
  static constexpr ara::core::Array<ara::core::ErrorCode, 4> Activate_PossibleErrors =
  {
    ::ara::ucm::pkgmgr::MakeErrorCode(::ara::ucm::pkgmgr::UCMErrorDomainErrc::kErrorDuringActivation, 0),
    ::ara::ucm::pkgmgr::MakeErrorCode(::ara::ucm::pkgmgr::UCMErrorDomainErrc::kErrorNoValidProcessing, 0),
    ::ara::ucm::pkgmgr::MakeErrorCode(::ara::ucm::pkgmgr::UCMErrorDomainErrc::kMissingDependencies, 0),
    ::ara::ucm::pkgmgr::MakeErrorCode(::ara::ucm::pkgmgr::UCMErrorDomainErrc::kOperationNotPermitted, 0),
  };

  static constexpr ara::core::Array<ara::core::ErrorCode, 3> Cancel_PossibleErrors =
  {
    ::ara::ucm::pkgmgr::MakeErrorCode(::ara::ucm::pkgmgr::UCMErrorDomainErrc::kCancelFailed, 0),
    ::ara::ucm::pkgmgr::MakeErrorCode(::ara::ucm::pkgmgr::UCMErrorDomainErrc::kOperationNotPermitted, 0),
    ::ara::ucm::pkgmgr::MakeErrorCode(::ara::ucm::pkgmgr::UCMErrorDomainErrc::kInvalidTransferId, 0),
  };

  static constexpr ara::core::Array<ara::core::ErrorCode, 4> DeleteTransfer_PossibleErrors =
  {
    ::ara::ucm::pkgmgr::MakeErrorCode(::ara::ucm::pkgmgr::UCMErrorDomainErrc::kGeneralReject, 0),
    ::ara::ucm::pkgmgr::MakeErrorCode(::ara::ucm::pkgmgr::UCMErrorDomainErrc::kGeneralMemoryError, 0),
    ::ara::ucm::pkgmgr::MakeErrorCode(::ara::ucm::pkgmgr::UCMErrorDomainErrc::kInvalidTransferId, 0),
    ::ara::ucm::pkgmgr::MakeErrorCode(::ara::ucm::pkgmgr::UCMErrorDomainErrc::kOperationNotPermitted, 0),
  };

  static constexpr ara::core::Array<ara::core::ErrorCode, 2> Finish_PossibleErrors =
  {
    ::ara::ucm::pkgmgr::MakeErrorCode(::ara::ucm::pkgmgr::UCMErrorDomainErrc::kGeneralReject, 0),
    ::ara::ucm::pkgmgr::MakeErrorCode(::ara::ucm::pkgmgr::UCMErrorDomainErrc::kOperationNotPermitted, 0),
  };

  static constexpr ara::core::Array<ara::core::ErrorCode, 0> GetHistory_PossibleErrors =
  {
  };

  struct GetHistoryOutput {
    ::ara::ucm::pkgmgr::GetHistoryVectorType history;

    using IsEnumerableTag = void;
    template<typename F>
    void enumerate(F& fun) {
      fun(history);
    }
  };
  static constexpr ara::core::Array<ara::core::ErrorCode, 1> GetLog_PossibleErrors =
  {
    ::ara::ucm::pkgmgr::MakeErrorCode(::ara::ucm::pkgmgr::UCMErrorDomainErrc::kInvalidTransferId, 0),
  };

  struct GetLogOutput {
    ::ara::ucm::pkgmgr::LogVectorType log;

    using IsEnumerableTag = void;
    template<typename F>
    void enumerate(F& fun) {
      fun(log);
    }
  };
  static constexpr ara::core::Array<ara::core::ErrorCode, 0> GetSwClusterChangeInfo_PossibleErrors =
  {
  };

  struct GetSwClusterChangeInfoOutput {
    ::ara::ucm::pkgmgr::SwClusterInfoVectorType SwInfo;

    using IsEnumerableTag = void;
    template<typename F>
    void enumerate(F& fun) {
      fun(SwInfo);
    }
  };
  static constexpr ara::core::Array<ara::core::ErrorCode, 0> GetSwClusterInfo_PossibleErrors =
  {
  };

  struct GetSwClusterInfoOutput {
    ::ara::ucm::pkgmgr::SwClusterInfoVectorType SwInfo;

    using IsEnumerableTag = void;
    template<typename F>
    void enumerate(F& fun) {
      fun(SwInfo);
    }
  };
  static constexpr ara::core::Array<ara::core::ErrorCode, 0> GetSwPackages_PossibleErrors =
  {
  };

  struct GetSwPackagesOutput {
    ::ara::ucm::pkgmgr::SwPackageInfoVectorType Packages;

    using IsEnumerableTag = void;
    template<typename F>
    void enumerate(F& fun) {
      fun(Packages);
    }
  };
  static constexpr ara::core::Array<ara::core::ErrorCode, 1> GetSwProcessProgress_PossibleErrors =
  {
    ::ara::ucm::pkgmgr::MakeErrorCode(::ara::ucm::pkgmgr::UCMErrorDomainErrc::kInvalidTransferId, 0),
  };

  struct GetSwProcessProgressOutput {
    std::uint8_t progress;

    using IsEnumerableTag = void;
    template<typename F>
    void enumerate(F& fun) {
      fun(progress);
    }
  };
  static constexpr ara::core::Array<ara::core::ErrorCode, 8> ProcessSwPackage_PossibleErrors =
  {
    ::ara::ucm::pkgmgr::MakeErrorCode(::ara::ucm::pkgmgr::UCMErrorDomainErrc::kServiceBusy, 0),
    ::ara::ucm::pkgmgr::MakeErrorCode(::ara::ucm::pkgmgr::UCMErrorDomainErrc::kInvalidManifest, 0),
    ::ara::ucm::pkgmgr::MakeErrorCode(::ara::ucm::pkgmgr::UCMErrorDomainErrc::kProcessedSoftwarePackageInconsistent, 0),
    ::ara::ucm::pkgmgr::MakeErrorCode(::ara::ucm::pkgmgr::UCMErrorDomainErrc::kInsufficientMemory, 0),
    ::ara::ucm::pkgmgr::MakeErrorCode(::ara::ucm::pkgmgr::UCMErrorDomainErrc::kInvalidTransferId, 0),
    ::ara::ucm::pkgmgr::MakeErrorCode(::ara::ucm::pkgmgr::UCMErrorDomainErrc::kOperationNotPermitted, 0),
    ::ara::ucm::pkgmgr::MakeErrorCode(::ara::ucm::pkgmgr::UCMErrorDomainErrc::kProcessSwPackageCancelled, 0),
    ::ara::ucm::pkgmgr::MakeErrorCode(::ara::ucm::pkgmgr::UCMErrorDomainErrc::kAuthenticationFailed, 0),
  };

  static constexpr ara::core::Array<ara::core::ErrorCode, 3> RevertProcessedSwPackages_PossibleErrors =
  {
    ::ara::ucm::pkgmgr::MakeErrorCode(::ara::ucm::pkgmgr::UCMErrorDomainErrc::kNothingToRevert, 0),
    ::ara::ucm::pkgmgr::MakeErrorCode(::ara::ucm::pkgmgr::UCMErrorDomainErrc::kNotAbleToRevertPackages, 0),
    ::ara::ucm::pkgmgr::MakeErrorCode(::ara::ucm::pkgmgr::UCMErrorDomainErrc::kOperationNotPermitted, 0),
  };

  static constexpr ara::core::Array<ara::core::ErrorCode, 3> Rollback_PossibleErrors =
  {
    ::ara::ucm::pkgmgr::MakeErrorCode(::ara::ucm::pkgmgr::UCMErrorDomainErrc::kNothingToRollback, 0),
    ::ara::ucm::pkgmgr::MakeErrorCode(::ara::ucm::pkgmgr::UCMErrorDomainErrc::kNotAbleToRollback, 0),
    ::ara::ucm::pkgmgr::MakeErrorCode(::ara::ucm::pkgmgr::UCMErrorDomainErrc::kOperationNotPermitted, 0),
  };

  static constexpr ara::core::Array<ara::core::ErrorCode, 1> SetLogLevel_PossibleErrors =
  {
    ::ara::ucm::pkgmgr::MakeErrorCode(::ara::ucm::pkgmgr::UCMErrorDomainErrc::kInvalidTransferId, 0),
  };

  static constexpr ara::core::Array<ara::core::ErrorCode, 7> TransferData_PossibleErrors =
  {
    ::ara::ucm::pkgmgr::MakeErrorCode(::ara::ucm::pkgmgr::UCMErrorDomainErrc::kAuthenticationFailed, 0),
    ::ara::ucm::pkgmgr::MakeErrorCode(::ara::ucm::pkgmgr::UCMErrorDomainErrc::kIncorrectBlock, 0),
    ::ara::ucm::pkgmgr::MakeErrorCode(::ara::ucm::pkgmgr::UCMErrorDomainErrc::kIncorrectSize, 0),
    ::ara::ucm::pkgmgr::MakeErrorCode(::ara::ucm::pkgmgr::UCMErrorDomainErrc::kInsufficientMemory, 0),
    ::ara::ucm::pkgmgr::MakeErrorCode(::ara::ucm::pkgmgr::UCMErrorDomainErrc::kInvalidTransferId, 0),
    ::ara::ucm::pkgmgr::MakeErrorCode(::ara::ucm::pkgmgr::UCMErrorDomainErrc::kOperationNotPermitted, 0),
    ::ara::ucm::pkgmgr::MakeErrorCode(::ara::ucm::pkgmgr::UCMErrorDomainErrc::kPackageInconsistent, 0),
  };

  static constexpr ara::core::Array<ara::core::ErrorCode, 6> TransferExit_PossibleErrors =
  {
    ::ara::ucm::pkgmgr::MakeErrorCode(::ara::ucm::pkgmgr::UCMErrorDomainErrc::kInsufficientData, 0),
    ::ara::ucm::pkgmgr::MakeErrorCode(::ara::ucm::pkgmgr::UCMErrorDomainErrc::kPackageInconsistent, 0),
    ::ara::ucm::pkgmgr::MakeErrorCode(::ara::ucm::pkgmgr::UCMErrorDomainErrc::kAuthenticationFailed, 0),
    ::ara::ucm::pkgmgr::MakeErrorCode(::ara::ucm::pkgmgr::UCMErrorDomainErrc::kOldVersion, 0),
    ::ara::ucm::pkgmgr::MakeErrorCode(::ara::ucm::pkgmgr::UCMErrorDomainErrc::kInvalidTransferId, 0),
    ::ara::ucm::pkgmgr::MakeErrorCode(::ara::ucm::pkgmgr::UCMErrorDomainErrc::kOperationNotPermitted, 0),
  };

  static constexpr ara::core::Array<ara::core::ErrorCode, 1> TransferStart_PossibleErrors =
  {
    ::ara::ucm::pkgmgr::MakeErrorCode(::ara::ucm::pkgmgr::UCMErrorDomainErrc::kInsufficientMemory, 0),
  };

  struct TransferStartOutput {
    ::ara::ucm::pkgmgr::TransferIdType id;

    using IsEnumerableTag = void;
    template<typename F>
    void enumerate(F& fun) {
      fun(id);
    }
  };
};

} // namespace pkgmgr
} // namespace ucm
} // namespace ara

#endif // _home_user_Desktop_GP_VALEO_AP_ara_project_R19_11_ara_api_ucm_pkgmgr_Build_gen_includes_ara_ucm_pkgmgr_packagemanagement_common_h

