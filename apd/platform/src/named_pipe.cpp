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

#include <string>
#include <sstream>
#include <cstring>
#include <unistd.h>
#include <algorithm>


#include "apd/platform/named_pipe.h"
#include "apd/platform/errno_exception.h"
#include "apd/platform/open_pipe_exception.h"

namespace apd {
namespace platform {

NamedPipe::NamedPipe(const std::string & pathname, Direction direction, bool blocking)
    : pathname_(pathname) {
  int flags(0);
  switch (direction) {
    case READ:
      // Set O_NONBLOCK on read sockets so we can open all read sockets without waiting for the counterpart.
      // The flag will be removed after the call to open()
      flags |= O_RDONLY | O_NONBLOCK;
      break;
    case WRITE:
      flags |= O_WRONLY;
      break;
  }

  pipe_ = open(pathname.c_str(), flags);
  if (-1 == pipe_) {
    throw exception::OpenPipeException(errno);
  }

  if (READ == direction && blocking == true) {
    // Remove the O_NONBLOCK flag
    flags &= ~O_NONBLOCK;
    if (-1 == fcntl(pipe_, F_SETFL, flags)) {
      pipe_ = INVALID_FD;
      throw exception::OpenPipeException(errno);
    }
  }
}

NamedPipe::NamedPipe(NamedPipe&& other) : pathname_(std::move(other.pathname_)), pipe_(other.pipe_) {
  other.pipe_ = INVALID_FD;
}

NamedPipe::~NamedPipe() {
  if (pipe_ > 0) {
    ::close(pipe_);
  }
}

} /* namespace platform */
} /* namespace apd */
