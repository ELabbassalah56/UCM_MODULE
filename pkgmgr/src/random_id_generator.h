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

#ifndef ARA_UCM_PKGMGR_RANDOM_ID_GENERATOR_H_
#define ARA_UCM_PKGMGR_RANDOM_ID_GENERATOR_H_

#include <cstdint>
#include <iterator>
#include <algorithm>
#include <type_traits>
#include <limits>
#include <random>
#include "ara/core/optional.h"
#include "serial_id_generator.h"

namespace ara
{
namespace ucm
{
namespace pkgmgr
{

/// @brief Random transfer id generator class.
template <typename Id>
class RandomIDGenerator
{
public:
    using ResultType = Id;

    static_assert(std::is_unsigned<Id>::value, "Unsigned arithmetic type required as Id type for RandomIDGenerator.");

    /// @brief Generate a new ID based on all existing ones
    ///
    /// @param ids Array of existing ids
    ///
    /// @return New key or nothing in case of reaching the limit
    template <class Container, std::enable_if_t<std::is_same<typename Container::value_type, Id>::value, int> = 0>
    ara::core::Optional<Id> operator()(Container const& ids)
    {

        for (unsigned int i = 0; i < attempts_; ++i) {
            auto id = dis_(gen_);

            if (std::find(std::begin(ids), std::end(ids), id) == std::end(ids))
                return id;
        }
        // fallback to serial generation
        return SerialIDGenerator<Id>{}(ids);
    }

    /// @brief Generate a new ID based on all existing ones
    /// used for Map-like associative containers
    ///
    /// @param ids Array of existing ids
    ///
    /// @return New key or nothing in case of reaching the limit
    template <class Container,
        std::enable_if_t<std::is_same<typename Container::key_type, Id>::value
                && std::is_same<typename Container::value_type::first_type, const Id>::value
                && std::is_same<typename Container::key_compare, std::less<Id>>::value,
            int> = 0>
    ara::core::Optional<Id> operator()(Container const& ids)
    {
        for (unsigned int i = 0; i < attempts_; ++i) {
            auto id = dis_(gen_);

            if (ids.find(id) == std::end(ids))
                return id;
        }
        // fallback to serial generation
        return SerialIDGenerator<Id>{}(ids);
    }

private:
    /// @brief Random number engine
    std::mt19937_64 gen_{std::random_device{}()};
    /// @brief Uniform distribution with parametes 1 and std::numeric_limits<Id>::max()
    std::uniform_int_distribution<Id> dis_{1};
    /// @brief Number of attempts
    static constexpr unsigned int attempts_{10};
};

}  // namespace pkgmgr
}  // namespace ucm
}  // namespace ara

#endif  // ARA_UCM_PKGMGR_RANDOM_ID_GENERATOR_H_
