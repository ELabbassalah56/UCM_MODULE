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

#ifndef ARA_UCM_PKGMGR_RECEIVE_RECEIVE_ERROR_H_
#define ARA_UCM_PKGMGR_RECEIVE_RECEIVE_ERROR_H_

#include "ara/core/error_domain.h"
#include "ara/core/error_code.h"
#include "ara/core/exception.h"

using ara::core::ErrorDomain;
using ara::core::ErrorCode;

namespace ara
{
namespace ucm
{
namespace pkgmgr
{

/// @uptrace{SWS_UCM_00136}
enum class receive_errc : ErrorDomain::CodeType
{
    invalid_transfer_id = 1,
    transfer_not_complete
};

class ReceiveException : public ara::core::Exception
{
public:
    ReceiveException(ara::core::ErrorCode&& err)
        : ara::core::Exception(std::move(err))
    {}
};

#if defined(__GNUC__)
#    pragma GCC diagnostic push
#    pragma GCC diagnostic ignored "-Wnon-virtual-dtor"
#endif

class ReceiveErrorDomain : public ErrorDomain
{
    constexpr static ErrorDomain::IdType kId = 0x42;

public:
    constexpr ReceiveErrorDomain() noexcept
        : ErrorDomain(kId)
    {}
    char const* Name() const noexcept override
    {
        return "UCM Receive Package";
    }
    char const* Message(ErrorDomain::CodeType errorCode) const noexcept override
    {
        receive_errc const code = static_cast<receive_errc>(errorCode);
        switch (code) {
        case receive_errc::invalid_transfer_id:
            return "Invalid transfer Id";
        case receive_errc::transfer_not_complete:
            return "Transfer is not complete";
        default:
            return "Unknown error";
        }
    }

    void ThrowAsException(ErrorCode const& errorCode) const noexcept(false) override
    {
        ara::core::ThrowOrTerminate<ReceiveException>(errorCode);
    }
};

namespace internal
{
constexpr ReceiveErrorDomain g_receiveErrorDomain;
}

inline constexpr ErrorDomain const& GetReceiveErrorDomain()
{
    return internal::g_receiveErrorDomain;
}

inline constexpr ErrorCode MakeErrorCode(receive_errc code, ErrorDomain::SupportDataType data)
{
    return ErrorCode(static_cast<ErrorDomain::CodeType>(code), GetReceiveErrorDomain(), data);
}

}  // namespace pkgmgr
}  // namespace ucm
}  // namespace ara

#endif  // ARA_UCM_PKGMGR_RECEIVE_RECEIVE_ERROR_H_
