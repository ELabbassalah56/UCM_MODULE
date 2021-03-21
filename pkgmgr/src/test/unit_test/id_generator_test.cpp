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

#include "gtest/gtest.h"

#include "serial_id_generator.h"
#include "random_id_generator.h"
#include "ara/core/vector.h"
#include "ara/core/map.h"
#include "ara/core/string.h"

#include <cstdint>
#include <limits>

namespace ara
{
namespace ucm
{
namespace pkgmgr
{

TEST(IdGeneratorTestSuite, SerialIDGenerationKeyContainer)
{

    SerialIDGenerator<std::uint64_t> gen;

    ara::core::Vector<std::uint64_t> emptyIdVec{};
    ara::core::Vector<std::uint64_t> idVec{2, 1};
    ara::core::Vector<std::uint64_t> idVecGap{2, 1, 7};

    auto start = gen(emptyIdVec);

    ASSERT_TRUE(start);
    ASSERT_EQ(*start, 1);

    auto id = gen(idVec);

    ASSERT_TRUE(id);
    ASSERT_EQ(*id, 3);

    id = gen(idVecGap);

    ASSERT_TRUE(id);
    ASSERT_EQ(*id, 3);
}

TEST(IdGeneratorTestSuite, SerialIDGenerationMap)
{

    SerialIDGenerator<std::uint64_t> gen;

    ara::core::Map<std::uint64_t, ara::core::String> emptyIdMap{};
    ara::core::Map<std::uint64_t, ara::core::String> idMap{{2, "first"}, {1, "second"}};
    ara::core::Map<std::uint64_t, ara::core::String> idMapGap{{2, "first"}, {1, "second"}, {7, "seventh"}};

    auto start = gen(emptyIdMap);

    ASSERT_TRUE(start);
    ASSERT_EQ(*start, 1);

    auto id = gen(idMap);

    ASSERT_TRUE(id);
    ASSERT_EQ(*id, 3);

    id = gen(idMapGap);

    ASSERT_TRUE(id);
    ASSERT_EQ(*id, 3);
}

TEST(IdGeneratorTestSuite, RandomIDGenerationKeyContainer)
{

    RandomIDGenerator<std::uint64_t> gen;

    ara::core::Vector<std::uint64_t> emptyIdVec{};
    ara::core::Vector<std::uint64_t> idVec{2, 1};
    ara::core::Vector<std::uint64_t> idVecGap{2, 1, 7};

    ASSERT_TRUE(gen(emptyIdVec));
    ASSERT_TRUE(gen(idVec));
    ASSERT_TRUE(gen(idVecGap));
}

TEST(IdGeneratorTestSuite, RandomIDGenerationMap)
{

    RandomIDGenerator<std::uint64_t> gen;

    ara::core::Map<std::uint64_t, ara::core::String> emptyIdMap{};
    ara::core::Map<std::uint64_t, ara::core::String> idMap{{2, "first"}, {1, "second"}};
    ara::core::Map<std::uint64_t, ara::core::String> idMapGap{{2, "first"}, {1, "second"}, {7, "seventh"}};

    ASSERT_TRUE(gen(emptyIdMap));
    ASSERT_TRUE(gen(idMap));
    ASSERT_TRUE(gen(idMapGap));
}

TEST(IdGeneratorTestSuite, IDGenerationFullContainer)
{
    using id_type = std::uint8_t;

    ara::core::Vector<id_type> fullIdVec;

    fullIdVec.reserve(std::numeric_limits<id_type>::max());
    for (id_type value = 0; value < std::numeric_limits<id_type>::max(); ++value) {
        fullIdVec.push_back(value + 1);
    }

    SerialIDGenerator<id_type> serialGen;
    RandomIDGenerator<id_type> randomGen;

    ASSERT_FALSE(serialGen(fullIdVec));
    ASSERT_FALSE(randomGen(fullIdVec));

    ara::core::Map<id_type, ara::core::String> fullIdMap;
    for (id_type value = 0; value < std::numeric_limits<id_type>::max(); ++value) {
        fullIdMap.emplace(value + 1, "value");
    }

    ASSERT_FALSE(serialGen(fullIdMap));
    ASSERT_FALSE(randomGen(fullIdMap));
}

}  // namespace pkgmgr
}  // namespace ucm
}  // namespace ara
