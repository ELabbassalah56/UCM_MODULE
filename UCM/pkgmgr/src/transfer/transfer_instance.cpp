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

#include "transfer_instance.h"
#include "receive_types.h"

#include "boost/filesystem.hpp"

#include <fcntl.h>

namespace ara
{
namespace ucm
{
namespace pkgmgr
{

TransferInstance::TransferInstance(TransferIdType id, const ara::core::String path)
    : transferId_(id)
    , receivedBytes_(0)
    , expectedBytes_()
    , state_(TransferState::Idle)
    , expectedBlock_(1)
    , fd_()
{
    path_ = path;
}

TransferStartReturnType TransferInstance::TransferStart(const std::uint32_t& size)
{
    ara::core::String tmpPath = GetPackagePath();
    fd_ = open(tmpPath.c_str(), O_RDWR | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);

    switch (posix_fallocate(fd_, 0, size)) {
    case EBADF:
    case EFBIG:
    case EINTR:
    case EINVAL:
    case ENODEV:
    case ESPIPE:  // no return value for these errors
    case ENOSPC:  // error: no enough space
        unlink(
            tmpPath.c_str());  // delete file: though ENOSPC is returned, the file with different size will be created
        return {transferId_, TransferStartSuccessType::kInsufficientMemory};
        break;
    }

    state_ = TransferState::Transferring;
    expectedBytes_ = size;

    return {transferId_, TransferStartSuccessType::kSuccess};
}

TransferDataReturnType TransferInstance::TransferData(const ByteVectorType& data, const std::uint32_t& blockCounter)
{
    if (state_ != TransferState::Transferring) {
        return TransferDataReturnType::kOperationNotPermitted;
    }

    if (expectedBlock_ != blockCounter) {  // when block is different
        return TransferDataReturnType::kIncorrectBlock;
    }

    if (expectedBytes_ < receivedBytes_ + data.size()) {  // when the stored data overall
        return TransferDataReturnType::kIncorrectSize;
    }

    write(fd_, &data[0], data.size());

    expectedBlock_++;
    receivedBytes_ += data.size();

    return TransferDataReturnType::kSuccess;
}

TransferExitReturnType TransferInstance::TransferExit()
{
    close(fd_);

    if (state_ != TransferState::Transferring) {
        return TransferExitReturnType::kOperationNotPermitted;
    }

    if (expectedBytes_ == receivedBytes_) {
        state_ = TransferState::Idle;
        return TransferExitReturnType::kSuccess;
    }

    return TransferExitReturnType::kInsufficientData;
}

ara::core::String TransferInstance::GetPackagePath() const
{
    return path_ + "/" + std::to_string(transferId_) + ".zip";
}

DeleteTransferReturnType TransferInstance::DeleteTransfer()
{
    ara::core::String path = GetPackagePath();
    if (remove(path.c_str()) == 0) {
        return DeleteTransferReturnType::kSuccess;
    } else {
        return DeleteTransferReturnType::kGeneralMemoryError;
    }
}

}  // namespace pkgmgr
}  // namespace ucm
}  // namespace ara
