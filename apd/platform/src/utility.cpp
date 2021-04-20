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

#include <limits.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/time.h>
#include <string>
#include <vector>
#include <sstream>

#include "apd/platform/utility.h"
#include "apd/platform/errno_exception.h"


namespace apd {
namespace platform {
namespace utility {

void Sleep(uint32_t time_ms) {
    struct timespec ts;
    ts.tv_sec = time_ms / 1000;
    ts.tv_nsec = (time_ms % 1000) * 1000000;
    nanosleep(&ts, NULL);
}

std::string getWorkingDirectory() {
    char path[PATH_MAX];
    if (getcwd(path, PATH_MAX) == NULL) {
        throw exception::ErrnoException(errno);
    }
    return std::string(path);
}

void trim(std::string & text, const std::string & chars_to_trim) {
  size_t first_char(text.find_first_not_of(chars_to_trim));
  size_t last_char(text.find_last_not_of(chars_to_trim));
  if (first_char == 0 && last_char == text.length() - 1) {
    return;
  } else if ((first_char == std::string::npos && last_char == std::string::npos) || (last_char < first_char)) {
    // This string is either empty or contains nothing but trimmable characters
    text = std::string();
  } else {
    text = text.substr(first_char, last_char - first_char + 1);
  }
}



StringVector addSpaces(const StringVector & input, size_t max_space_count) {
  StringVector additional_lines;
  additional_lines.reserve(max_space_count * max_space_count);

  for (StringVector::const_iterator key_value_it(input.begin()); key_value_it != input.end(); ++key_value_it) {
    for (size_t pre_space_count(0); pre_space_count <= max_space_count; ++pre_space_count) {
      for (size_t post_space_count(0); post_space_count <= max_space_count; ++post_space_count) {
        if (0 != pre_space_count || 0 != post_space_count) {
          std::ostringstream stream;
          for (size_t i(0); i < pre_space_count; ++i) {
            stream << " ";
          }
          stream << *key_value_it;
          for (size_t i(0); i < post_space_count; ++i) {
            stream << " ";
          }
          additional_lines.emplace_back(stream.str());
        }
      }
    }
  }
  return additional_lines;
}

uint64_t getSystemTimeUS() {
    struct timeval locTime;
    gettimeofday(&locTime, 0);
    return (locTime.tv_sec * (uint64_t) 1000000 + locTime.tv_usec);
}

} /* namespace utility */
} /* namespace platform */
} /* namespace apd */
