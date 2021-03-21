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

#ifndef ARA_UCM_PKGMGR_TRANSFER_TRANSFER_INSTANCE_H_
#define ARA_UCM_PKGMGR_TRANSFER_TRANSFER_INSTANCE_H_

#include "streamable.h"

namespace ara
{
namespace ucm
{
namespace pkgmgr
{

/// @brief This class composes a file from vectors of bytes according to initially passed size.
/// It also check errors
///
/// @uptrace{RS_UCM_00019}
/// @uptrace{RS_UCM_00025}
class TransferInstance : public Streamable
{
public:
    explicit TransferInstance(TransferIdType, const ara::core::String);

    /// <inheritdoc>
    TransferStartReturnType TransferStart(const std::uint32_t& size);

    /// <inheritdoc>
    TransferDataReturnType TransferData(const ByteVectorType& data, const std::uint32_t& blockCounter);

    /// <inheritdoc>
    TransferExitReturnType TransferExit();

    /// <inheritdoc>
    ara::core::String GetPackagePath() const;

    /// <inheritdoc>
    DeleteTransferReturnType DeleteTransfer();

private:
    enum class TransferState : uint8_t
    {
        Idle = 0,
        Transferring = 1,
    };

    void InitPath();

    TransferIdType transferId_;
    unsigned int receivedBytes_;
    ara::core::String path_;
    unsigned int expectedBytes_;
    TransferState state_;
    std::uint32_t expectedBlock_;
    int fd_;
};

}  // namespace pkgmgr
}  // namespace ucm
}  // namespace ara

#endif  // ARA_UCM_PKGMGR_TRANSFER_TRANSFER_INSTANCE_H_
