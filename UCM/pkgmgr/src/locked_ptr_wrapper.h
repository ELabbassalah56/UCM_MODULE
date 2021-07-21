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

#ifndef ARA_UCM_PKGMGR_LOCKED_PTR_WRAPPER_H
#define ARA_UCM_PKGMGR_LOCKED_PTR_WRAPPER_H

#include <mutex>
#include <memory>
#include <functional>

namespace ara
{
namespace ucm
{
namespace pkgmgr
{

/// @brief This class represents a pointer wrapper
/// that owns and manages another object
/// what can be accessed in the exclusive mode
///
/// @tparam T pointer type
///
template <typename T>
class LockedPtrWrapper
{
    using UniquePtr = std::unique_ptr<T>;
    using ConstReference = const T&;
    using ChangeHandler = std::function<void(ConstReference)>;

    /// @brief Guard to synchronize pointer access
    std::mutex m_;

    /// @brief Internat pointer
    UniquePtr inner_;

    /// @brief Handler function what is called on internal pointer reset
    ChangeHandler handler_;

public:
    /// @brief Initializes a new instance of this service with the given instance id
    ///
    /// @param ptr unique pointer to object to be stored
    /// @param handler function to be called on internal pointer reset
    LockedPtrWrapper(UniquePtr ptr, ChangeHandler handler = ChangeHandler())
        : inner_(std::move(ptr))
        , handler_(handler)
    {}

    // copy and move operations are deleted
    LockedPtrWrapper(const LockedPtrWrapper&) = delete;
    LockedPtrWrapper(LockedPtrWrapper&&) = delete;
    LockedPtrWrapper& operator=(const LockedPtrWrapper&) = delete;
    LockedPtrWrapper& operator=(LockedPtrWrapper&&) = delete;

    /// @brief Accessor class for managed object access in exclusive mode
    class Accessor
    {
        /// @brief Internal guard for access synchronization
        std::unique_lock<std::mutex> guard_;

        /// @brief A reference to wrapper object
        std::reference_wrapper<LockedPtrWrapper> ptrWrapper_;

    public:
        /// @brief Creates an accessor
        ///
        /// @param lptr a pointer wrapper reference
        Accessor(LockedPtrWrapper& lptr)
            : guard_(lptr.m_)
            , ptrWrapper_(lptr)
        {}

        // copy constructor is deleted
        Accessor(const Accessor&) = delete;

        /// @brief move constructor
        ///
        /// @param other object to be moved
        Accessor(Accessor&& other)
            : guard_(std::move(other.guard_))
            , ptrWrapper_(std::move(other.ptrWrapper_))
        {}

        // copy assignment operator is deleted
        Accessor& operator=(const Accessor&) = delete;

        /// @brief Move assignment operator
        ///
        /// @param rhs object to be moved
        Accessor& operator=(Accessor&& rhs)
        {
            guard_ = std::move(rhs.guard_);
            ptrWrapper_ = std::move(rhs.ptrWrapper_);
        }

        /// @brief Provieds access to the object owned by wrapper
        ///
        /// @return A poineter to the object owned by wrapper
        T* operator->() const
        {
            return ptrWrapper_.get().inner_.get();
        }

        /// @brief Replaces the managed object
        ///
        /// @param prt pointer to a new object to manage
        void Reset(UniquePtr ptr)
        {   
            ptrWrapper_.get().inner_ = std::move(ptr);
            if (ptrWrapper_.get().handler_) {
                ptrWrapper_.get().handler_(*ptrWrapper_.get().inner_.get());
            }
        }
    };

    /// @brief Gets an access to a managed object
    ///
    /// @return Accessor object (used to call methods of managed object or replace object)
    Accessor Get()
    {
        return {*this};
    }
};

}  // namespace pkgmgr
}  // namespace ucm
}  // namespace ara

#endif  // ARA_UCM_PKGMGR_LOCKED_PTR_WRAPPER_H
