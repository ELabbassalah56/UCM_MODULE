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

#include <apd/crc/buffer.h>
#include <stdexcept>
#include <iomanip>
#include <cctype>

namespace apd
{

namespace crc
{

BufferView::BufferView(const uint8_t* ptr, size_t length)
    : dataPtr_{ptr}
    , dataLength_{length}
{
    if (!isValid()) {
        throw std::invalid_argument{"data is incorrect"};
    }
}

BufferView::BufferView(const Buffer& buf, size_t length)
    : dataPtr_{buf.data()}
    , dataLength_{length}
{
    if (length > buf.size() || length == 0) {
        throw std::length_error{"Length is incorrect."};
    }
}

BufferView::BufferView(const Buffer& buf, size_t begin, size_t end)
    : dataPtr_{buf.data() + begin}
    , dataLength_(end - begin)
{
    if (begin > end) {
        throw std::invalid_argument{"Incorrect buffer boundaries (begin > end)."};
    }
    if (end > buf.size()) {
        throw std::out_of_range{"Length is incorrect (end > buf.size())."};
    }
}

BufferView::BufferView(BufferView&& oth) noexcept : dataPtr_{oth.dataPtr_}, dataLength_{oth.dataLength_}
{
    oth.dataPtr_ = nullptr;
    oth.dataLength_ = 0U;
}

BufferView& BufferView::operator=(BufferView&& oth) noexcept
{
    if (this != &oth) {
        dataPtr_ = oth.dataPtr_;
        dataLength_ = oth.dataLength_;
        oth.dataPtr_ = nullptr;
        oth.dataLength_ = 0U;
    }
    return *this;
}

bool BufferView::isValid() const noexcept
{
    if (dataLength_ == 0) {
        return true;
    }

    return dataPtr_ != nullptr;
}

}  // namespace crc
}  // namespace apd

std::ostream& operator<<(std::ostream& os, const apd::crc::Buffer& buffer)
{
    for (auto&& byte : buffer) {
        if (std::isupper(byte) != 0) {
            os << byte;
        } else {
            os << "[" << std::setfill('0') << std::setw(2) << std::hex << static_cast<uint32_t>(byte) << std::dec
               << "]";
        }
    }
    return os;
}
