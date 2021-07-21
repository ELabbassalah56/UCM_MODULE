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

#ifndef ARA_UCM_PKGMGR_EXCEPTIONS_H_
#define ARA_UCM_PKGMGR_EXCEPTIONS_H_

#include "ara/core/string.h"

namespace ara
{
namespace ucm
{
namespace pkgmgr
{
namespace exception
{

/// @brief Base class for all logic related errors within UCM.
class UCMLogicError : public std::logic_error
{
public:
    /// @brief Initialize this exception.
    ///
    /// @param message The error message
    UCMLogicError(const ara::core::String& message)
        : std::logic_error(message.c_str())
    {}
};

/// @brief Exception for malformed manifest files.
///
/// The exception is thrown if a manifest has a wrong format or cannot be found
class InvalidManifestException : public UCMLogicError
{
public:
    /// @brief Initialize this exception.
    ///
    /// @param message The error message
    InvalidManifestException(const ara::core::String& message)
        : UCMLogicError(message)
    {}
};
}  // namespace exception
}  // namespace pkgmgr
}  // namespace ucm
}  // namespace ara

#endif  // ARA_UCM_PKGMGR_EXCEPTIONS_H_
