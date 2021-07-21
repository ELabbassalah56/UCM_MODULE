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

#include "version.h"

namespace ara
{
namespace ucm
{
namespace pkgmgr
{

Version::Version(const uint32_t vmajor, const uint32_t vminor)
    : vmajor_{vmajor}
    , vminor_{vminor}
    , versionString_(
          ([](uint32_t a, uint32_t b) { return ara::core::String(std::to_string(a) + '.' + std::to_string(b)); })  //
          (vmajor, vminor))
{}

Version::Version(const ara::core::String& version)
    : versionString_{version}
{
    int succAssignedValues = std::sscanf(version.c_str(), "%u.%u", &vmajor_, &vminor_);
    if (succAssignedValues != 2) {
        std::cerr << "version " + version + " does not match format %u.%u" << std::endl;
        throw std::invalid_argument(("version " + version + " does not match format %u.%u").c_str());
    }
}

bool Version::operator<(const Version& other) const
{
    return versionString_ < other.versionString_;
}

bool Version::operator==(const Version& other) const
{
    return versionString_ == other.versionString_;
}

bool Version::operator>(const Version& other) const
{
    return versionString_ > other.versionString_;
}

bool Version::operator<=(const Version& other) const
{
    return versionString_ <= other.versionString_;
}

bool Version::operator>=(const Version& other) const
{
    return versionString_ >= other.versionString_;
}

ara::core::String Version::ToString() const
{
    return versionString_;
}

inline std::ostream& operator<<(std::ostream& stream, const Version& ver)
{
    return stream << ver.ToString();
}

}  // namespace pkgmgr
}  // namespace ucm
}  // namespace ara
