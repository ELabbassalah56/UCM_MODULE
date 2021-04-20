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
#include <algorithm>
#include <gtest/gtest.h>
#include <apd/crc/buffer.h>

namespace buffer = apd::crc;

TEST(BufferTestSuite, BufferViewCreation)
{
    bool areEqual = false;
    buffer::Buffer data{0x00U, 0x01U, 0x02U, 0x03U};
    buffer::BufferView view1{data};
    areEqual = std::equal(data.begin(), data.end(), view1.begin(), view1.end());
    EXPECT_EQ(areEqual, true);

    buffer::BufferView view2{data, 2};
    areEqual = std::equal(data.begin(), data.begin() + 2, view2.begin(), view2.end());
    EXPECT_EQ(areEqual, true);

    buffer::BufferView view3{data, 1, 3};
    areEqual = std::equal(data.begin() + 1, data.begin() + 3, view3.begin(), view3.end());
    EXPECT_EQ(areEqual, true);

    buffer::BufferView view4{view1};
    areEqual = std::equal(view1.begin(), view1.end(), view4.begin(), view4.end());
    EXPECT_EQ(areEqual, true);

    buffer::BufferView view5{std::move(view1)};
    EXPECT_EQ(view1.begin(), nullptr);
    areEqual = std::equal(view4.begin(), view4.end(), view5.begin(), view5.end());
    EXPECT_EQ(areEqual, true);

    buffer::BufferView view6{data};
    view6 = view5;
    areEqual = std::equal(view4.begin(), view4.end(), view5.begin(), view5.end());
    EXPECT_EQ(areEqual, true);

    view6 = std::move(view6);
    EXPECT_FALSE(view6.begin() == nullptr);
}
