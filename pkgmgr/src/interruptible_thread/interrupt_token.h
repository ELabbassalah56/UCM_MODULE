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

#ifndef ARA_UCM_PKGMGR_INTERRUPTIBLE_THREAD_INTERRUPT_TOKEN_H
#define ARA_UCM_PKGMGR_INTERRUPTIBLE_THREAD_INTERRUPT_TOKEN_H

#include <memory>
#include <atomic>

namespace ara
{
namespace ucm
{
namespace pkgmgr
{

/// @brief A class represents a token which is used to
/// interrupt thread execution
class InterruptToken
{
public:
    /// @brief Creates a token with a given initial state
    ///
    /// @param initialState Initial token state
    explicit InterruptToken(bool initialState);

    /// @brief Set the token internal state to interrupted
    ///
    /// @return The previous token state
    bool Interrupt() noexcept;

    /// @brief Checks if the token internal state is set to interrupted
    ///
    /// @return True if the token internal state is set to interrupted, false otherwise
    bool IsInterrupted() const noexcept;

private:
    /// @brief A shared pointer to atomic bool
    /// represents the token internal state and synchronizes the access
    std::shared_ptr<std::atomic<bool>> internal_;
};

}  // namespace pkgmgr
}  // namespace ucm
}  // namespace ara

#endif  // ARA_UCM_PKGMGR_INTERRUPTIBLE_THREAD_INTERRUPT_TOKEN_H
