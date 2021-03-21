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

#include <unistd.h>
#include <csignal>
#include <cstdint>
#include <atomic>
#include "ara/exec/execution_client.h"
#include "ara/log/logging.h"
#include "adaptive_application.h"

namespace ara
{
namespace ucm
{
namespace pkgmgr
{

// File descriptors for pipe ends
int AdaptiveApplication::selfpipe[2] = {0, 0};

// Atomic flag for exit after SIGTERM caught, ctor is constexpr
std::atomic_bool AdaptiveApplication::exit_requested_(false);

void AdaptiveApplication::SigTermHandler(int sig)
{
    if (sig == SIGTERM) {
        // set atomic exit flag
        exit_requested_ = true;
        // write to self pipe
        write(selfpipe[PipeWritingEnd], "\0", 1);
    }
}

bool AdaptiveApplication::OnInitialize()
{
    
    return true;
}

void AdaptiveApplication::OnTerminate()
{
    
}

void AdaptiveApplication::WaitUntilTermination()
{
    // self pipe trick waiting
    while (!exit_requested_) {
        // reading from self pipe
        int buf;
        read(selfpipe[PipeReadingEnd], &buf, sizeof(buf));
    }
}

void AdaptiveApplication::ReportRunningState()
{
    ara::exec::ExecutionClient exec_client;
    exec_client.ReportExecutionState(ara::exec::ExecutionState::kRunning);
}

int AdaptiveApplication::Execute()
{
    if (Initialize()) {
        Run();
        Terminate();
        return EXIT_SUCCESS;
    }
    return EXIT_FAILURE;
}

bool AdaptiveApplication::Initialize()
{
    if (!OnInitialize())
        return false;
    // Creating a pipe
    if (pipe(selfpipe) == -1) {
        // Could not create a pipe for performing a classic self pipe trick
        log_.LogError() << "Execution stopped. Unable to create a pipe";
        return false;
    }
    struct sigaction sa;
    sa.sa_handler = SigTermHandler;
    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);
    // register signal handler
    if (sigaction(SIGTERM, &sa, NULL) == -1) {
        // Could not register a SIGTERM signal handler
        log_.LogError() << "Execution stopped. Unable to register signal handler";
        CloseSelfPipe();
        return false;
    }
    return true;
}

void AdaptiveApplication::ReportTerminatingState()
{
    ara::exec::ExecutionClient exec_client;
    exec_client.ReportExecutionState(ara::exec::ExecutionState::kTerminating);
}

void AdaptiveApplication::Terminate()
{
    ReportTerminatingState();
    OnTerminate();
    CloseSelfPipe();
}

void AdaptiveApplication::CloseSelfPipe()
{
    close(selfpipe[PipeReadingEnd]);
    close(selfpipe[PipeWritingEnd]);
}

}  // namespace pkgmgr
}  // namespace ucm
}  // namespace ara
