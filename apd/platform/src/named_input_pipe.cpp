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

#include <cerrno>
#include "apd/platform/named_input_pipe.h"
#include <apd/platform/errno_exception.h>

namespace apd {
namespace platform {

NamedInputPipe::NamedInputPipe(NamedInputPipe&& other) : NamedPipe(std::move(other)),
    buffer_(std::move(other.buffer_)),
    bytes_in_buffer_(other.bytes_in_buffer_),
    blocking_(other.blocking_) {
    other.bytes_in_buffer_ = 0;
}

void NamedInputPipe::pop_bytes(size_t num_bytes) {
  if (bytes_in_buffer_ > 0) {
    // Compute how much will remain in the buffer
    size_t remaining_in_buffer = bytes_in_buffer_ - num_bytes;
    if (num_bytes > bytes_in_buffer_) {
      remaining_in_buffer = 0;
    }

    if (remaining_in_buffer > 0) {
      // move the remaining bytes around
      // std::memmove(&buffer_[0], &buffer_[bytes_in_buffer_], remaining_in_buffer);
      buffer_.erase(buffer_.begin(), buffer_.begin() + num_bytes);
    }
    // Mark the buffer as shorter;
    bytes_in_buffer_ = remaining_in_buffer;
  }
}

void NamedInputPipe::read_bytes(size_t max_bytes) {
  if (bytes_in_buffer_ < buffer_.size()) {
    size_t bytes_requested(std::min(buffer_.size() - bytes_in_buffer_, max_bytes));
    // Note: Implementation will fail silently, if the buffer is of insufficient length.
    ssize_t bytes_read(::read(pipe_, &buffer_[bytes_in_buffer_], bytes_requested));
    if (-1 == bytes_read) {
        if (blocking_ || errno != EAGAIN) {
            throw exception::ErrnoException(errno);
        }
    } else {
        bytes_in_buffer_ += bytes_read;
    }
  }
}

template<> bool NamedInputPipe::read<std::string>(std::string & buffer) {
  int i;
  while(true) {
      i = 0;
      while (bytes_in_buffer_) {
        // Copy all chars till null-character
        if (buffer_[i] == '\0') {
            pop_bytes(i + 1);
            return true;
        }

        buffer += buffer_[i++];
        bytes_in_buffer_--;
      }
      read_bytes(INPUT_BUFFER_LENGTH);
      if (bytes_in_buffer_ == 0 && buffer.empty()) {
          return false;
      }
  }
}

} /* namespace platform */
} /* namespace apd */
