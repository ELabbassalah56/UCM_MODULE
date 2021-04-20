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

#include <cstdint>
#include <apd/crc/crc.h>
#include <gtest/gtest.h>

namespace buffer = apd::crc;
namespace crc = apd::crc;

TEST(CRCTestSuite, CRC_CalculateCRC64_0)
{
    /* given */
    buffer::Buffer data = {0x00, 0x00, 0x00, 0x00};
    uint64_t test_p7_crc_0{0xF4A586351E1B9F4BU};

    /* when */
    uint64_t calculatedCrc{crc::CRC::CalculateCRC64(buffer::BufferView(data))};

    /* then */
    EXPECT_EQ(calculatedCrc, test_p7_crc_0);
}

TEST(CRCTestSuite, CRC_CalculateCRC64_1)
{
    /* given */
    buffer::Buffer data = {0xF2, 0x01, 0x83};
    uint64_t test_p7_crc_1 = 0x319C27668164F1C6U;

    /* when */
    uint64_t calculatedCrc = crc::CRC::CalculateCRC64(buffer::BufferView(data));
    /* then */
    EXPECT_EQ(calculatedCrc, test_p7_crc_1);
}

TEST(CRCTestSuite, CRC_CalculateCRC64_2)
{
    /* given */
    buffer::Buffer data = {0x0F, 0xAA, 0x00, 0x55};
    uint64_t test_p7_crc_2 = 0x54C5D0F7667C1575U;

    /* when */
    uint64_t calculatedCrc = crc::CRC::CalculateCRC64(buffer::BufferView(data));

    /* then */
    EXPECT_EQ(calculatedCrc, test_p7_crc_2);
}

TEST(CRCTestSuite, CRC_CalculateCRC64_3)
{
    /* given */
    buffer::Buffer data = {0x00, 0xFF, 0x55, 0x11};
    uint64_t test_p7_crc_3 = 0xA63822BE7E0704E6U;

    /* when */
    uint64_t calculatedCrc = crc::CRC::CalculateCRC64(buffer::BufferView(data));

    /* then */
    EXPECT_EQ(calculatedCrc, test_p7_crc_3);
}

TEST(CRCTestSuite, CRC_CalculateCRC64_4)
{
    /* given */
    buffer::Buffer data = {0x33, 0x22, 0x55, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF};
    uint64_t test_p7_crc_4 = 0x701ECEB219A8E5D5U;

    /* when */
    uint64_t calculatedCrc = crc::CRC::CalculateCRC64(buffer::BufferView(data));

    /* then */
    EXPECT_EQ(calculatedCrc, test_p7_crc_4);
}

TEST(CRCTestSuite, CRC_CalculateCRC64_5)
{
    /* given */
    buffer::Buffer data = {0x92, 0x6B, 0x55};
    uint64_t test_p7_crc_5 = 0x5FAA96A9B59F3E4EU;

    /* when */
    uint64_t calculatedCrc = crc::CRC::CalculateCRC64(buffer::BufferView(data));

    /* then */
    EXPECT_EQ(calculatedCrc, test_p7_crc_5);
}

TEST(CRCTestSuite, CRC_CalculateCRC64_6)
{
    /* given */
    buffer::Buffer data = {0xFF, 0xFF, 0xFF, 0xFF};
    uint64_t test_p7_crc_6 = 0xFFFFFFFF00000000U;

    /* when */
    uint64_t calculatedCrc = crc::CRC::CalculateCRC64(buffer::BufferView(data));

    /* then */
    EXPECT_EQ(calculatedCrc, test_p7_crc_6);
}
