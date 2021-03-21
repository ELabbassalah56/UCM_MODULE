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

#ifndef ARA_UCM_PKGMGR_SYNCHRONIZED_STORAGE_H_
#define ARA_UCM_PKGMGR_SYNCHRONIZED_STORAGE_H_

#include "ara/com/types.h"
#include "ara/core/map.h"
#include "ara/core/vector.h"
#include "ara/core/optional.h"

#include <boost/thread/shared_mutex.hpp>

#include <algorithm>
#include <type_traits>

namespace ara
{
namespace ucm
{
namespace pkgmgr
{

/// @brief This class is a synchronized map
///
/// @uptrace{SWS_UCM_00075}
template <typename Key, typename ValueType>
class SynchronizedStorage
{
public:
    using Value = std::shared_ptr<ValueType>;

    SynchronizedStorage()
    {}

    virtual ~SynchronizedStorage()
    {}

    /// @brief add item to the container
    ///
    /// @param id Key where to insert a new value
    /// @param data Value to insert
    void AddItem(Key id, Value data);

    /// @brief Get item from the container assotiated with the key
    ///
    /// @param id identifier of an item to get
    ///
    /// @return Value or nullptr in case of an error
    virtual Value GetItem(Key id) const;

    /// @brief Create and store new item
    ///
    /// @param factory Item factory to use
    ///
    /// @return Newly inserted pair or error
    template <typename Factory, typename Generator>
    ara::core::Optional<std::pair<Key, Value>> CreateAndStoreItem(Factory& factory, Generator& generator);

    /// @brief delete item from the container
    ///
    /// @param id Key of a value to delete
    void DeleteItem(Key id);

    /// @brief Apply function to every item in caontainer
    ///
    /// @param processor Function to apply to key-value pairs
    void IterateItems(std::function<void(const std::pair<Key, Value const&>)> processor) const;

private:
    mutable boost::shared_mutex access_;
    ara::core::Map<Key, Value> data_;

    void NonBlockingAddItem(Key id, Value data);
};

template <typename Key, typename ValueType>
using TValue = typename SynchronizedStorage<Key, ValueType>::Value;

template <typename Key, typename ValueType>
void SynchronizedStorage<Key, ValueType>::AddItem(Key id, std::shared_ptr<ValueType> data)
{
    boost::unique_lock<boost::shared_mutex> lock{access_};

    NonBlockingAddItem(id, data);
}

template <typename Key, typename ValueType>
typename SynchronizedStorage<Key, ValueType>::Value SynchronizedStorage<Key, ValueType>::GetItem(Key id) const
{
    boost::shared_lock<boost::shared_mutex> lock{access_};

    auto iter = data_.find(id);

    if (iter == data_.end())
        return nullptr;

    return iter->second;
}

template <typename Key, typename ValueType>
template <typename Factory, typename Generator>
ara::core::Optional<std::pair<Key, TValue<Key, ValueType>>> SynchronizedStorage<Key, ValueType>::CreateAndStoreItem(
    Factory& factory,
    Generator& generator)
{
    static_assert(std::is_same<typename Generator::ResultType, Key>::value,
        "Generated Result Type shall be equal to Key data type");

    boost::unique_lock<boost::shared_mutex> lock{access_};

    auto key = generator(data_);

    if (!key) {
        return {};
    }

    TValue<Key, ValueType> item = factory.Create(*key);
    NonBlockingAddItem(*key, item);

    return {std::make_pair(item->GetID(), item)};
}

template <typename Key, typename ValueType>
void SynchronizedStorage<Key, ValueType>::DeleteItem(Key id)
{
    boost::unique_lock<boost::shared_mutex> lock{access_};

    data_.erase(id);
}

template <typename Key, typename ValueType>
void SynchronizedStorage<Key, ValueType>::IterateItems(
    std::function<void(const std::pair<Key, TValue<Key, ValueType> const&>)> processor) const
{
    boost::unique_lock<boost::shared_mutex> lock{access_};

    for (auto pair : data_) {
        processor(pair);
    }
}

template <typename Key, typename ValueType>
void SynchronizedStorage<Key, ValueType>::NonBlockingAddItem(Key id,
    typename SynchronizedStorage<Key, ValueType>::Value data)
{
    data_.insert(std::make_pair(id, data));
}

}  // namespace pkgmgr
}  // namespace ucm
}  // namespace ara

#endif  // ARA_UCM_PKGMGR_SYNCHRONIZED_STORAGE_H_
