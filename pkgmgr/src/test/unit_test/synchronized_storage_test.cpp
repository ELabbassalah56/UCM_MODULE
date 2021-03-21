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

#include "synchronized_storage.h"
#include "serial_id_generator.h"
#include <dirent.h>
#include "test_helper.h"
#include <fstream>
#include <iostream>
#include <cstdint>
#include "ara/log/logging.h"

#include "transfer/software_package_data_factory.h"

namespace ara
{
namespace ucm
{
namespace pkgmgr
{

using ::testing::Eq;
using ::testing::Ne;
using ::testing::StrEq;

class SynchronizedStorageTestFixture : public ::testing::Test
{
public:
    SynchronizedStorageTestFixture()
    {}

    void SetUp()
    {}

    void TearDown()
    {}

    SynchronizedStorage<std::uint64_t, StreamableSoftwarePackage> data_;
};

TEST_F(SynchronizedStorageTestFixture, GetItem_Success)
{
    std::uint64_t key = 1;
    auto value = std::make_shared<SoftwarePackageData>(3);

    data_.AddItem(key, value);
    auto item = data_.GetItem(key);

    ASSERT_TRUE(item != nullptr);
    ASSERT_TRUE(item->GetID() == value->GetID());
}

TEST_F(SynchronizedStorageTestFixture, GetItem_Fail)
{
    std::uint64_t key = 1;

    auto item = data_.GetItem(key);

    ASSERT_THAT(item, nullptr);
}

TEST_F(SynchronizedStorageTestFixture, CreateAndStoreItem_Success)
{
    SerialIDGenerator<std::uint64_t> gen;
    SoftwarePackageDataFactory factory;

    auto item = data_.CreateAndStoreItem(factory, gen);

    ASSERT_TRUE(item);

    auto v = *item;

    ASSERT_THAT(v.first, 1);
    ASSERT_THAT(v.second->GetID(), v.first);
    ASSERT_THAT(v.second->GetState(), SwPackageStateType::kTransferring);
}

TEST_F(SynchronizedStorageTestFixture, DeleteItem_Success)
{
    std::uint64_t key = 1;
    std::shared_ptr<SoftwarePackageData> value = std::make_shared<SoftwarePackageData>(3);
    data_.AddItem(key, value);
    data_.DeleteItem(key);

    auto item = data_.GetItem(key);

    ASSERT_FALSE(item);
}

TEST_F(SynchronizedStorageTestFixture, DeleteItem_Fail)
{
    std::uint64_t key = 1;
    std::shared_ptr<SoftwarePackageData> value = std::make_shared<SoftwarePackageData>(3);
    data_.AddItem(key, value);

    data_.DeleteItem(key + 1);

    auto item = data_.GetItem(key);

    ASSERT_TRUE(item);
}

TEST_F(SynchronizedStorageTestFixture, IterateItems)
{
    std::uint64_t key = 1;
    std::shared_ptr<SoftwarePackageData> value1 = std::make_shared<SoftwarePackageData>(3);
    data_.AddItem(key, value1);

    std::uint64_t key2 = 1000;
    std::shared_ptr<SoftwarePackageData> value2 = std::make_shared<SoftwarePackageData>(4);
    data_.AddItem(key2, value2);

    std::uint64_t sumKeys = 0;
    TransferIdType sumValues = 0;

    data_.IterateItems(
        [&sumKeys, &sumValues](const std::pair<std::uint64_t, std::shared_ptr<StreamableSoftwarePackage> const&> in) {
            sumKeys += in.first;
            sumValues += in.second->GetID();
        });

    ASSERT_TRUE(sumKeys == key + key2);
    ASSERT_TRUE(sumValues == value1->GetID() + value2->GetID());
}

}  //  namespace pkgmgr
}  //  namespace ucm
}  //  namespace ara
