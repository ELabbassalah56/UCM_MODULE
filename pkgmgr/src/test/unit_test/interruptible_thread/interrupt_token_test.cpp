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

#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "interruptible_thread/interrupt_token.h"

namespace ara
{
namespace ucm
{
namespace pkgmgr
{

class InterruptTokenFixture : public ::testing::Test
{
protected:
    InterruptToken token_{false};
    InterruptToken trueInitializedToken_{true};
};

TEST_F(InterruptTokenFixture, Initialization)
{
    ASSERT_EQ(token_.IsInterrupted(), false);
    ASSERT_EQ(trueInitializedToken_.IsInterrupted(), true);
}

TEST_F(InterruptTokenFixture, Interruption)
{
    // Check for the previous value
    ASSERT_EQ(token_.Interrupt(), false);
    ASSERT_EQ(trueInitializedToken_.Interrupt(), true);
    // Check for the new value
    ASSERT_EQ(token_.IsInterrupted(), true);
    ASSERT_EQ(trueInitializedToken_.IsInterrupted(), true);
}

TEST_F(InterruptTokenFixture, CopyInterruption)
{
    // Create new token from copy constructor
    InterruptToken copyCtorToken = token_;

    InterruptToken copyAssignToken{true};
    copyAssignToken = token_;

    token_.Interrupt();

    // Check for the new value
    ASSERT_EQ(copyCtorToken.IsInterrupted(), true);
    ASSERT_EQ(copyAssignToken.IsInterrupted(), true);
}

}  // namespace pkgmgr
}  // namespace ucm
}  // namespace ara
