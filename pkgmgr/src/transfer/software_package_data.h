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

#ifndef ARA_UCM_PKGMGR_TRANSFER_SOFTWARE_PACKAGE_DATA_H_
#define ARA_UCM_PKGMGR_TRANSFER_SOFTWARE_PACKAGE_DATA_H_

#include "ara/ucm/pkgmgr/impl_type_transferidtype.h"
#include "ara/ucm/pkgmgr/impl_type_swpackagestatetype.h"
#include "ara/ucm/pkgmgr/impl_type_logleveltype.h"
#include "streamable_software_package.h"

#include <mutex>

namespace ara
{
namespace ucm
{
namespace pkgmgr
{

/// @brief Class for storing data about software package in work
class SoftwarePackageData : public StreamableSoftwarePackage
{
public:
    explicit SoftwarePackageData(TransferIdType id);

    virtual ~SoftwarePackageData() = default;
    SoftwarePackageData(const SoftwarePackageData&) = default;
    SoftwarePackageData(SoftwarePackageData&&) = default;
    SoftwarePackageData& operator=(const SoftwarePackageData&) = default;
    SoftwarePackageData& operator=(SoftwarePackageData&&) = default;

    /// <inheritdoc>
    TransferIdType const& GetID() const override
    {
        return transferID_;
    }

    /// <inheritdoc>
    void SetState(SwPackageStateType state) override
    {
        std::lock_guard<std::mutex> guard(access_);

        logger_.AddEntry("Move to state " + ara::core::String(std::to_string(static_cast<int>(state))));
        state_ = state;
    }

    /// <inheritdoc>
    SwPackageStateType const& GetState() const override
    {
        return state_;
    }

    /// <inheritdoc>
    TransferStartReturnType TransferStart(const std::uint32_t& size) override
    {
        std::lock_guard<std::mutex> guard(access_);

        logger_.AddEntry("TransferStart() start");
        auto result = GetStreamable()->TransferStart(size);
        logger_.AddEntry("TransferStart() finish");

        return result;
    }

    /// <inheritdoc>
    TransferDataReturnType TransferData(const ByteVectorType& data, const std::uint32_t& blockCounter) override
    {
        std::lock_guard<std::mutex> guard(access_);

        logger_.AddEntry("TransferData(" + ara::core::String(std::to_string(blockCounter)) + ") start");
        auto result = GetStreamable()->TransferData(data, blockCounter);
        logger_.AddEntry("TransferData(" + ara::core::String(std::to_string(blockCounter)) + ") finish");

        return result;
    }

    /// <inheritdoc>
    TransferExitReturnType TransferExit() override
    {
        std::lock_guard<std::mutex> guard(access_);

        logger_.AddEntry("TransferStart() start");
        auto result = GetStreamable()->TransferExit();
        logger_.AddEntry("TransferStart() finish");

        return result;
    }

    /// <inheritdoc>
    ara::core::String GetPackagePath() const override
    {
        return GetStreamable()->GetPackagePath();
    }

    /// <inheritdoc>
    Logger& GetLogger() override
    {
        return logger_;
    }

    /// <inheritdoc>
    DeleteTransferReturnType DeleteTransfer() override
    {
        std::lock_guard<std::mutex> guard(access_);

        logger_.AddEntry("DeleteTransfer() start");
        auto result = GetStreamable()->DeleteTransfer();
        logger_.AddEntry("DeleteTransfer() finish");

        return result;
    }

private:
    /// @brief Id of the object
    TransferIdType transferID_;

    /// @brief state of the object
    SwPackageStateType state_;

    /// @brief part of package responsible for receiving transferring data
    std::unique_ptr<Streamable> transfer_;

    /// @brief protect transfer_ and state_ from simultaneous access
    std::mutex access_;

    /// @brief object to keep log messages related to this package
    Logger logger_;

    virtual std::unique_ptr<Streamable>& GetStreamable()
    {
        return transfer_;
    }

    virtual std::unique_ptr<Streamable> const& GetStreamable() const
    {
        return transfer_;
    }
};

}  // namespace pkgmgr
}  // namespace ucm
}  // namespace ara

#endif  // ARA_UCM_PKGMGR_TRANSFER_SOFTWARE_PACKAGE_DATA_H_
