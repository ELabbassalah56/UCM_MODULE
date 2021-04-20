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

TEST(CRCTestSuite, CRC_CalculateCRC16_0)
{
    /* given */
    buffer::Buffer data = {0x00, 0x00, 0x00, 0x00};
    uint32_t expected = 0x84C0U;

    /* when */
    uint32_t calculatedCrc = crc::CRC::CalculateCRC16(buffer::BufferView(data));

    /* then */
    EXPECT_EQ(calculatedCrc, expected);
}

TEST(CRCTestSuite, CRC_CalculateCRC16_1)
{
    /* given */
    buffer::Buffer data = {0xF2, 0x01, 0x83};
    uint32_t expected = 0xD374U;

    /* when */
    uint32_t calculatedCrc = crc::CRC::CalculateCRC16(buffer::BufferView(data));
    /* then */
    EXPECT_EQ(calculatedCrc, expected);
}

TEST(CRCTestSuite, CRC_CalculateCRC16_2)
{
    /* given */
    buffer::Buffer data = {0x0F, 0xAA, 0x00, 0x55};
    uint32_t expected = 0x2023U;

    /* when */
    uint32_t calculatedCrc = crc::CRC::CalculateCRC16(buffer::BufferView(data));

    /* then */
    EXPECT_EQ(calculatedCrc, expected);
}

TEST(CRCTestSuite, CRC_CalculateCRC16_3)
{
    /* given */
    buffer::Buffer data = {0x00, 0xFF, 0x55, 0x11};
    uint32_t expected = 0xB8F9U;

    /* when */
    uint32_t calculatedCrc = crc::CRC::CalculateCRC16(buffer::BufferView(data));

    /* then */
    EXPECT_EQ(calculatedCrc, expected);
}

TEST(CRCTestSuite, CRC_CalculateCRC16_4)
{
    /* given */
    buffer::Buffer data = {0x33, 0x22, 0x55, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF};
    uint32_t expected = 0xF53FU;

    /* when */
    uint32_t calculatedCrc = crc::CRC::CalculateCRC16(buffer::BufferView(data));

    /* then */
    EXPECT_EQ(calculatedCrc, expected);
}

TEST(CRCTestSuite, CRC_CalculateCRC16_5)
{
    /* given */
    buffer::Buffer data = {0x92, 0x6B, 0x55};
    uint32_t expected = 0x0745U;

    /* when */
    uint32_t calculatedCrc = crc::CRC::CalculateCRC16(buffer::BufferView(data));

    /* then */
    EXPECT_EQ(calculatedCrc, expected);
}

TEST(CRCTestSuite, CRC_CalculateCRC16_6)
{
    /* given */
    buffer::Buffer data = {0xFF, 0xFF, 0xFF, 0xFF};
    uint32_t expected = 0x1D0FU;

    /* when */
    uint32_t calculatedCrc = crc::CRC::CalculateCRC16(buffer::BufferView(data));

    /* then */
    EXPECT_EQ(calculatedCrc, expected);
}
