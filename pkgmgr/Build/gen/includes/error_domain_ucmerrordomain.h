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

#ifndef _home_user_Desktop_GP_VALEO_AP_ara_project_R19_11_ara_api_ucm_pkgmgr_Build_gen_includes_error_domain_ucmerrordomain_h
#define _home_user_Desktop_GP_VALEO_AP_ara_project_R19_11_ara_api_ucm_pkgmgr_Build_gen_includes_error_domain_ucmerrordomain_h

#include "ara/core/error_code.h"
#include "ara/core/error_domain.h"
#include "ara/core/exception.h"

namespace ara {
namespace ucm {
namespace pkgmgr {


enum class UCMErrorDomainErrc : ara::core::ErrorDomain::CodeType
{
  kAuthenticationFailed = 8, ///< Software Package authentication failed.
  kCancelFailed = 16, ///< Cancel failed.
  kErrorDuringActivation = 19, ///< Activate failed.
  kErrorNoValidProcessing = 20, ///< Activate cannot be performed because previous processing is invalid.
  kGeneralMemoryError = 11, ///< A general memory error occured.
  kGeneralReject = 10, ///< General reject.
  kIncorrectBlock = 2, ///< The block counter value is not as expected.
  kIncorrectSize = 3, ///< The size of the Software Package exceeds the provided size in TransferStart.
  kInsufficientData = 6, ///< TransferExit has been called but total transferred data size does not match expected data size provided with TransferStart call.
  kInsufficientMemory = 1, ///< Insufficient memory to perform operation.
  kInvalidManifest = 13, ///< Package manifest could not be read.
  kInvalidTransferId = 4, ///< The Transfer ID is invalid.
  kMissingDependencies = 21, ///< Activate cannot be performed because of missing dependencies.
  kNotAbleToRevertPackages = 15, ///< RevertProcessedSwPackages failed.
  kNotAbleToRollback = 18, ///< Rollback failed.
  kNothingToRevert = 14, ///< RevertProcessedSwPackages has been called without prior processing of a Software Package.
  kNothingToRollback = 17, ///< Rollback cannot be performed due to no rollback data available.
  kOldVersion = 9, ///< Software Package version is too old.
  kOperationNotPermitted = 5, ///< The operation is not supported in the current context.
  kPackageInconsistent = 7, ///< Package integrity check failed.
  kProcessSwPackageCancelled = 22, ///< The processing operation has been interrupted by a Cancel() call.
  kProcessedSoftwarePackageInconsistent = 23, ///< The processed Software Package integrity check has failed.
  kServiceBusy = 12, ///< Another processing is already ongoing and therefore the current processing request has to be rejected.
};

class UCMErrorDomainException : public ara::core::Exception
{
public:
    explicit UCMErrorDomainException(ara::core::ErrorCode err) noexcept
        : ara::core::Exception(err)
    {}
};

class UCMErrorDomainErrorDomain final : public ara::core::ErrorDomain
{
    constexpr static ara::core::ErrorDomain::IdType kId = 42;

public:
    using Errc = UCMErrorDomainErrc;

    using Exception = UCMErrorDomainException;

    /// @brief Default constructor
    ///
    /// @uptrace{SWS_CORE_00241}
    /// @uptrace{SWS_CORE_00012}
    constexpr UCMErrorDomainErrorDomain() noexcept
        : ara::core::ErrorDomain(kId)
    {}

    /// @brief Return the "shortname" ApApplicationErrorDomain.SN of this error domain.
    char const* Name() const noexcept override
    {
        return "UCMErrorDomain";
    }

    char const* Message(ara::core::ErrorDomain::CodeType errorCode) const noexcept override
    {
        Errc const code = static_cast<Errc>(errorCode);
        switch (code) {
        case Errc::kAuthenticationFailed:
            return "Software Package authentication failed.";
        case Errc::kCancelFailed:
            return "Cancel failed.";
        case Errc::kErrorDuringActivation:
            return "Activate failed.";
        case Errc::kErrorNoValidProcessing:
            return "Activate cannot be performed because previous processing is invalid.";
        case Errc::kGeneralMemoryError:
            return "A general memory error occured.";
        case Errc::kGeneralReject:
            return "General reject.";
        case Errc::kIncorrectBlock:
            return "The block counter value is not as expected.";
        case Errc::kIncorrectSize:
            return "The size of the Software Package exceeds the provided size in TransferStart.";
        case Errc::kInsufficientData:
            return "TransferExit has been called but total transferred data size does not match expected data size provided with TransferStart call.";
        case Errc::kInsufficientMemory:
            return "Insufficient memory to perform operation.";
        case Errc::kInvalidManifest:
            return "Package manifest could not be read.";
        case Errc::kInvalidTransferId:
            return "The Transfer ID is invalid.";
        case Errc::kMissingDependencies:
            return "Activate cannot be performed because of missing dependencies.";
        case Errc::kNotAbleToRevertPackages:
            return "RevertProcessedSwPackages failed.";
        case Errc::kNotAbleToRollback:
            return "Rollback failed.";
        case Errc::kNothingToRevert:
            return "RevertProcessedSwPackages has been called without prior processing of a Software Package.";
        case Errc::kNothingToRollback:
            return "Rollback cannot be performed due to no rollback data available.";
        case Errc::kOldVersion:
            return "Software Package version is too old.";
        case Errc::kOperationNotPermitted:
            return "The operation is not supported in the current context.";
        case Errc::kPackageInconsistent:
            return "Package integrity check failed.";
        case Errc::kProcessSwPackageCancelled:
            return "The processing operation has been interrupted by a Cancel() call.";
        case Errc::kProcessedSoftwarePackageInconsistent:
            return "The processed Software Package integrity check has failed.";
        case Errc::kServiceBusy:
            return "Another processing is already ongoing and therefore the current processing request has to be rejected.";
        default:
            return "Unknown error";
        }
    }

    void ThrowAsException(ara::core::ErrorCode const& errorCode) const noexcept(false) override
    {
        ara::core::ThrowOrTerminate<Exception>(errorCode);
    }
};

namespace internal
{
constexpr UCMErrorDomainErrorDomain g_UCMErrorDomainErrorDomain;
}

inline constexpr ara::core::ErrorDomain const& GetUCMErrorDomainErrorDomain() noexcept
{
    return internal::g_UCMErrorDomainErrorDomain;
}

inline constexpr ara::core::ErrorCode MakeErrorCode(UCMErrorDomainErrc code,
    ara::core::ErrorDomain::SupportDataType data) noexcept
{
    return ara::core::ErrorCode(static_cast<ara::core::ErrorDomain::CodeType>(code), GetUCMErrorDomainErrorDomain(), data);
}

} // namespace pkgmgr
} // namespace ucm
} // namespace ara

#endif // _home_user_Desktop_GP_VALEO_AP_ara_project_R19_11_ara_api_ucm_pkgmgr_Build_gen_includes_error_domain_ucmerrordomain_h

