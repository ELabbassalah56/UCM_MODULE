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

#ifndef ARA_UCM_PKGMGR_TRANSFER_STREAMABLE_H_
#define ARA_UCM_PKGMGR_TRANSFER_STREAMABLE_H_

#include <cstdint>
#include "receive_types.h"
#include "ara/core/result.h"
#include "ara/core/string.h"

using ara::core::Result;

namespace ara
{
namespace ucm
{
namespace pkgmgr
{

using DeleteTransferReturnType = ::ara::ucm::pkgmgr::GeneralResponseType;

/// @brief Interface for object passed in parts
///
/// @uptrace{SWS_UCM_00007}
/// @uptrace{SWS_UCM_00088}
/// @uptrace{SWS_UCM_00140}
/// @uptrace{SWS_UCM_00008}
/// @uptrace{SWS_UCM_00010}
class Streamable
{
public:
    Streamable() = default;

    // Rule of five, cause we need virtual dtor for interface class
    virtual ~Streamable() = default;
    Streamable(Streamable const& other) = default;
    Streamable& operator=(Streamable const& other) = default;
    Streamable(Streamable&& other) = default;
    Streamable& operator=(Streamable&& other) = default;

    /// @brief Start the transfer of a Software Package.
    /// Transfer Id for subsequent calls to TransferData will be generated
    /// and returned as a part of TransferStartOutput.
    ///
    /// @param size Size (in bytes) of the Software Package to be transferred.
    ///
    /// @return The struct which contains the result of the transfer start operation
    /// and transfer id generated for this operation.
    virtual TransferStartReturnType TransferStart(const std::uint32_t& size) = 0;

    /// @brief Block-wise transfer of a Software Package.
    ///
    /// @param data Data block of the Software Package.
    /// @param blockCounter Block counter of the current block.
    ///
    /// @return The result of transferring current data block, which specifies if the
    /// sw package has been successfully transferred.
    virtual TransferDataReturnType TransferData(const ByteVectorType& data, const std::uint32_t& blockCounter) = 0;

    /// @brief Finish the transfer of a Software Package.
    ///
    /// @return The result of finishing this sw package transfer, which specifies if the
    /// sw package has been successfully transferred.
    virtual TransferExitReturnType TransferExit() = 0;

    /// @brief Receive the transferred data of the Software Package.
    ///
    /// @return The actual data which is transferred.
    virtual ara::core::String GetPackagePath() const = 0;

    /// @brief Delete a Software Package.
    ///
    /// @return The result of deleting sw package, which specifies if the
    /// sw package has been successfully deleted.
    virtual DeleteTransferReturnType DeleteTransfer() = 0;

    virtual void SetProcessProgressValue(std::uint8_t value)
    {
        processProgressValue = value;
    }

    virtual std::uint8_t GetProcessProgressValue() const
    {
        return processProgressValue;
    }

private:
    std::uint8_t processProgressValue;
};

}  // namespace pkgmgr
}  // namespace ucm
}  // namespace ara

#endif  // ARA_UCM_PKGMGR_TRANSFER_STREAMABLE_H_
