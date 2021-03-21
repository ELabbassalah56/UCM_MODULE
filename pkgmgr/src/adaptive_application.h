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

#ifndef ARA_UCM_PKGMGR_ADAPTIVE_APPLICATION_H_
#define ARA_UCM_PKGMGR_ADAPTIVE_APPLICATION_H_

#include <atomic>
#include "ara/log/logging.h"

namespace ara
{
namespace ucm
{
namespace pkgmgr
{

/// @brief This class represents
/// the Adaptive Application
///
/// see AUTOSAR_TR_Glossary.pdf
class AdaptiveApplication
{
public:
    AdaptiveApplication() = default;

    AdaptiveApplication(const AdaptiveApplication&) = delete;
    AdaptiveApplication& operator=(const AdaptiveApplication&) = delete;

    AdaptiveApplication(AdaptiveApplication&&) = delete;
    AdaptiveApplication& operator=(AdaptiveApplication&&) = delete;

    virtual ~AdaptiveApplication() = default;

    /// @brief A method represents the lifecycle of adaptive application.
    /// Shall be called from main() function (return app.Execute())
    ///
    /// @return Execution status
    ///(could be used as a return value from main() function)
    int Execute();

protected:
    /// @brief A method to be executed at initialize step.
    /// Default implementation simply returns true and does nothing
    /// Could be reimplemented for the specific application (if needed)
    ///
    /// @return Initialize success status (Was it successful or not)
    virtual bool OnInitialize();

    /// @brief A method to be executed at runtime.
    /// Shall be reimplemented for the specific application
    virtual void Run() = 0;

    /// @brief A method to be executed at terminate step.
    /// Default implementation simply does nothing
    /// Could be reimplemented for the specific application (if needed)
    virtual void OnTerminate();

    /// @brief Waiting until SIG_TERM will be received.
    /// Could be called from Run() method for specific
    /// application
    void WaitUntilTermination();

    /// @brief Reports that application went through the initialize process.
    /// This method shall be called from the Run() method to report
    /// that specific application reached running state
    void ReportRunningState();

private:
    /// @brief Performs at initialize step actions.
    /// Calls OnInitialize() which could be reimplemented bu specific application
    ///
    /// @return Initialize success status (Was it successful or not)
    bool Initialize();

    /// @brief Reports that application is in terminating state.
    void ReportTerminatingState();

    /// @brief Performs actions that are required during app termination
    /// Calls OnTerminate() which could be reimplemented by specific app
    void Terminate();

    /// @brief Close pipe ends for selfpipe.
    void CloseSelfPipe();

    // indexes for reading and writing ends of the pipe
    // it is better to use an enum, but CPP Guidelines forbids using unscoped enums
    // and scoped enums could not be used as array indexes directly

    /// @brief Index for reading end of the self pipe
    static constexpr int PipeReadingEnd = 0;

    /// @brief Index for writing end of the self pipe
    static constexpr int PipeWritingEnd = 1;

    /// @brief File descriptors for pipe ends (used by self pipe trick)
    static int selfpipe[2];

    /// @brief An atomic<bool> flag, which is used for exiting after SIGTERM caught
    static std::atomic_bool exit_requested_;

    /// @brief SIGTERM signal handler
    /// This function is used to handle SIGTERM signals.
    ///
    /// @param sig the signal, which is sent to application
    static void SigTermHandler(int sig);
    /// @brief A reference to logger, which is used to log app lifetime info
    ara::log::Logger& log_{ara::log::CreateLogger("APP", "Adaptive Application general context")};
};

}  // namespace pkgmgr
}  // namespace ucm
}  // namespace ara

#endif  // ARA_UCM_PKGMGR_ADAPTIVE_APPLICATION_H_
