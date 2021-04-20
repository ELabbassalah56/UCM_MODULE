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

TEST(CRCTestSuite, CRC_CalculateCRC8H2F_0)
{
    /* given */
    buffer::Buffer test_p22_data_0 = {0x00U, 0x00U, 0x00U, 0x00U};
    uint8_t test_p22_crc_0 = 0x12U;

    /* when */
    uint32_t calculatedCrc = crc::CRC::CalculateCRC8H2F(buffer::BufferView(test_p22_data_0));

    /* then */
    EXPECT_EQ(calculatedCrc, test_p22_crc_0);
}

TEST(CRCTestSuite, CRC_CalculateCRC8H2F_1)
{
    /* given */
    buffer::Buffer test_p22_data_1 = {0xF2U, 0x01U, 0x83U};
    uint8_t test_p22_crc_1 = 0xC2U;

    /* when */
    uint32_t calculatedCrc = crc::CRC::CalculateCRC8H2F(buffer::BufferView(test_p22_data_1));
    /* then */
    EXPECT_EQ(calculatedCrc, test_p22_crc_1);
}

TEST(CRCTestSuite, CRC_CalculateCRC8H2F_2)
{
    /* given */

    buffer::Buffer test_p22_data_2 = {0x0FU, 0xAAU, 0x00U, 0x55U};
    uint8_t test_p22_crc_2 = 0xC6U;
    /* when */
    uint32_t calculatedCrc = crc::CRC::CalculateCRC8H2F(buffer::BufferView(test_p22_data_2));

    /* then */
    EXPECT_EQ(calculatedCrc, test_p22_crc_2);
}

TEST(CRCTestSuite, CRC_CalculateCRC8H2F_3)
{
    /* given */
    buffer::Buffer test_p22_data_3 = {0x00U, 0xFFU, 0x55U, 0x11U};
    uint8_t test_p22_crc_3 = 0x77U;

    /* when */
    uint32_t calculatedCrc = crc::CRC::CalculateCRC8H2F(buffer::BufferView(test_p22_data_3));

    /* then */
    EXPECT_EQ(calculatedCrc, test_p22_crc_3);
}

TEST(CRCTestSuite, CRC_CalculateCRC8H2F_4)
{
    /* given */
    buffer::Buffer test_p22_data_4 = {0x33U, 0x22U, 0x55U, 0xAAU, 0xBBU, 0xCCU, 0xDDU, 0xEEU, 0xFFU};
    uint8_t test_p22_crc_4 = 0x11U;

    /* when */
    uint32_t calculatedCrc = crc::CRC::CalculateCRC8H2F(buffer::BufferView(test_p22_data_4));

    /* then */
    EXPECT_EQ(calculatedCrc, test_p22_crc_4);
}

TEST(CRCTestSuite, CRC_CalculateCRC8H2F_5)
{
    /* given */
    buffer::Buffer test_p22_data_5 = {0x92U, 0x6BU, 0x55U};
    uint8_t test_p22_crc_5 = 0x33U;

    /* when */
    uint32_t calculatedCrc = crc::CRC::CalculateCRC8H2F(buffer::BufferView(test_p22_data_5));

    /* then */
    EXPECT_EQ(calculatedCrc, test_p22_crc_5);
}

TEST(CRCTestSuite, CRC_CalculateCRC8H2F_6)
{
    /* given */
    buffer::Buffer test_p22_data_6 = {0xFFU, 0xFFU, 0xFFU, 0xFFU};
    uint8_t test_p22_crc_6 = 0x6CU;

    /* when */
    uint32_t calculatedCrc = crc::CRC::CalculateCRC8H2F(buffer::BufferView(test_p22_data_6));

    /* then */
    EXPECT_EQ(calculatedCrc, test_p22_crc_6);
}
