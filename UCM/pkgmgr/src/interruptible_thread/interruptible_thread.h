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

#ifndef ARA_UCM_PKGMGR_INTERRUPTIBLE_THREAD_INTERRUPTIBLE_THREAD_H
#define ARA_UCM_PKGMGR_INTERRUPTIBLE_THREAD_INTERRUPTIBLE_THREAD_H

#include "interrupt_token.h"
#include <thread>
#include <functional>

namespace ara
{
namespace ucm
{
namespace pkgmgr
{

/// @brief Class represents a thread of execution
/// which can be interrupted by setting the interruption token to
/// appropriate state
class InterruptibleThread
{
public:
    /// @brief Checks if the token (associated with this thread) internal state is set to interrupted
    ///
    /// @return True if the token (associated with this thread) internal state is set to interrupted, false otherwise
    inline static bool IsInterrupted()
    {
        return thread_it_.IsInterrupted();
    }

    /// @brief Constructs new thread object
    ///
    /// @param token associated interruption token
    /// @param f function to execute in the new thread
    /// @param args... arguments to pass to the new function
    template <typename Function, typename... Args>
    InterruptibleThread(InterruptToken token, Function&& f, Args&&... args)
        : thread_{[](InterruptToken t, auto&& fn, auto&&... arguments) {
                      // move to thread local
                      thread_it_ = std::move(t);

                      std::forward<decltype(fn)>(fn)(std::forward<decltype(arguments)>(arguments)...);
                  },
              token,
              std::forward<Function>(f),
              std::forward<Args>(args)...}
    {}

    /// @brief Waits for a thread to finish its execution
    void Join();

    /// @brief Permits the thread toexecute independently from the thread handle
    void Detach();

    /// @brief Checks whether the thread is joinable (potentially running in parallel context)
    /// @return True if the thread object identifies an ective thread of execution, false otherwise
    bool Joinable();

private:
    /// @brief The internal thread obect
    std::thread thread_{};

    /// @brief Thread local interrupt token(used to interrupt thread execution frm)
    static thread_local InterruptToken thread_it_;
};

}  // namespace pkgmgr
}  // namespace ucm
}  // namespace ara

#endif  // ARA_UCM_PKGMGR_INTERRUPTIBLE_THREAD_INTERRUPTIBLE_THREAD_H
