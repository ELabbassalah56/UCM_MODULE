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

#ifndef ARA_UCM_PKGMGR_PARSING_VERSION_H_
#define ARA_UCM_PKGMGR_PARSING_VERSION_H_

#include <cstdio>
#include <iostream>
#include "ara/core/string.h"
#include <stdexcept>

namespace ara
{
namespace ucm
{
namespace pkgmgr
{

/// @brief This class implements the version of an adaptive application
class Version
{
    /// @brief The major version number (the number before '.')
    uint32_t vmajor_;
    /// @brief The minor version number (the number after '.')
    uint32_t vminor_;
    /// @brief The version string.
    ///
    /// Temporary solution until the specification is clarified. See AR-93347 and AR-93345
    /// The value cannot be empty.
    ara::core::String versionString_;

public:
    /// @brief Initializes this version with vmajor.vminor
    ///
    /// @param vmajor The major version number
    /// @param vminor The minor version number
    Version(const uint32_t vmajor, const uint32_t vminor);

    /// @brief Initializes this version by parsing a version from a string
    ///
    /// @param version A version number in the format "%u.%u"
    ///
    /// @throws std::invalid_argument exception if the provided string does not match the excpected format
    explicit Version(const ara::core::String& version);

    /// @brief Version A < B if (A.major < B.major) else if (A.major == B.major)&&(A.minor < B.minor)
    ///
    /// @param other The Version to compare agains this version
    ///
    /// @returns true if *this < other, else false
    bool operator<(const Version& other) const;

    /// @brief Compares two Version objects.
    ///
    /// Two Version objects are equal if major and minor version numbers are equal
    ///
    /// @param other The Version to compare against this version
    ///
    /// @returns true if both Version objects are equal
    bool operator==(const Version& other) const;

    /// @brief Version A > B if !(A<=B)
    ///
    /// @param other The Version to compare against this version
    ///
    /// @returns true if this > other
    bool operator>(const Version& other) const;

    /// @brief Version A <= B if !(A>B)
    ///
    /// @param other The version to compare against this version
    ///
    /// @returns true if this <= other
    bool operator<=(const Version& other) const;

    /// @brief Version A >= B if !(A < B)
    ///
    /// @param other The version to compare against this version
    ///
    /// @returns true if this >= other
    bool operator>=(const Version& other) const;

    /// @brief Represents the version as a string in the format "vmajor.vminor"
    ///
    /// @returns The provided stream
    ara::core::String ToString() const;
};

/// @brief Prints the version the stream in the serialized format
///
/// @param stream The stream this version will be serialized to
/// @param ver The Version to serialize
///
/// @returns The provided stream
inline std::ostream& operator<<(std::ostream& stream, const Version& ver);

}  // namespace pkgmgr
}  // namespace ucm
}  // namespace ara

#endif  // ARA_UCM_PKGMGR_PARSING_VERSION_H_
