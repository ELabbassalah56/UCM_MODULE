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

#ifndef ARA_UCM_PKGMGR_SAMPLE_DATA_TRANSFER_H_
#define ARA_UCM_PKGMGR_SAMPLE_DATA_TRANSFER_H_

#include "ara/ucm/pkgmgr/packagemanagement_proxy.h"
#include <ara/log/logging.h>
#include <fstream>
#include <iostream>
#include <memory>
#include <vector>

namespace ara
{
namespace ucm
{
namespace pkgmgrsample
{

template <class PackageManagementService>
class DataTransfer
{
public:
    /// @brief Initializes this object with a reference to the PackageManagement
    /// service
    ///
    /// @param packageManagementService The instance of the PackageManagement
    /// service to be used
    /// @param blockSize The size of bytes for transmitting data blocks to UCM
    DataTransfer(std::shared_ptr<PackageManagementService> packageManagementService, uint32_t blockSize);
    virtual ~DataTransfer() = default;

    /// @brief Sets the block size of transmitted data blocks
    ///
    /// @param size the block size in number of bytes
    virtual void SetBlockSize(uint32_t size);

    /// @brief Initializes a new session
    ///
    /// @param size The size of the data that should be transmitted
    ///
    /// @return Boolean indicating whether the session was successfully initiated
    virtual bool InitSession(uint32_t size);

    /// @brief Transfers data to UCM sending the provided collection of bytes
    /// to UCM, possibly by using multiple smaller data blocks accoring to the
    /// configured block size.
    ///
    /// @param data The data that should be sent to UCM
    ///
    /// @return Boolean indicating whether the data was successfully sent
    virtual bool Transfer(const std::vector<uint8_t>& data);

    /// @brief Exits the current session
    ///
    /// @return Boolean indicating whether this session was successfully exited
    virtual bool ExitSession();

    /// @brief Delete SoftwarePackage
    ///
    /// @param transferId Id of the package to be deleted
    ///
    /// @returns Enum indicating whether deleting package was sucessful
    virtual bool DeleteSwPackage(const ara::ucm::pkgmgr::TransferIdType transferId);

    /// @brief Get SoftwarePackages
    ///
    /// @returns List of all Software Packages that have successfully transferd
    virtual pkgmgr::PackageManagement::GetSwPackagesOutput GetSwPackages();

    /// @brief Returns the session id of the current session.
    ///
    /// @return The id of the current session
    virtual pkgmgr::TransferIdType GetTransferId();

private:
    /// @brief The instance of the PackageManagement service used
    std::shared_ptr<PackageManagementService> packageManagementService_;

    /// @brief The configured block size for transmitting data
    uint32_t blockSize_;

    /// @brief The id of the current transfer
    pkgmgr::TransferIdType currentSession_;

    /// @brief For logging errors during transferring.
    log::Logger& logger_;
};

}  // namespace pkgmgrsample
}  // namespace ucm
}  // namespace ara

#endif  // ARA_UCM_PKGMGR_SAMPLE_DATA_TRANSFER_H_
