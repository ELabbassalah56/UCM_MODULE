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

#ifndef ARA_UCM_PKGMGR_TRANSFER_STREAMABLE_SOFTWARE_PACKAGE_H_
#define ARA_UCM_PKGMGR_TRANSFER_STREAMABLE_SOFTWARE_PACKAGE_H_

#include "streamable.h"
#include "ara/ucm/pkgmgr/impl_type_swpackagestatetype.h"
#include "ara/ucm/pkgmgr/impl_type_logvectortype.h"

// includes for Logger
#include "ara/core/string.h"
#include "ara/core/vector.h"
#include "ara/ucm/pkgmgr/impl_type_logleveltype.h"

using ara::core::Result;

namespace ara
{
namespace ucm
{
namespace pkgmgr
{

/// @brief Storage of log messages about particular StreamableSoftwarePackage with priorities
class Logger
{
public:
    Logger()
        : priority_(LogLevelType::kVerbose)
    {}

    /// @brief add log entry to storage
    ///
    /// @param log line to add
    /// @note for current implementation line priority will be got from Logger priority_ field
    void AddEntry(ara::core::String line)
    {
        logs_.push_back(LogEntryType({priority_, line}));
    }

    /// @brief get all logs
    ///
    /// @return vector of log entries with priorities
    LogVectorType const& GetLogs() const
    {
        return logs_;
    }

private:
    LogVectorType logs_;
    LogLevelType priority_;
};

/// @brief Interface of a streamable software package
class StreamableSoftwarePackage : public Streamable
{
public:
    /// @brief get id of the object
    ///
    /// @returns id of the object
    virtual TransferIdType const& GetID() const = 0;

    /// @brief set new state to this object
    ///
    /// @param state new state to set
    virtual void SetState(SwPackageStateType state) = 0;

    /// @brief get state of the package (e.g. kTransferring, kProcessed)
    ///
    /// @returns state of the object
    virtual SwPackageStateType const& GetState() const = 0;

    /// @brief Get logs for this Software Package
    ///
    /// @returns Logger object
    virtual Logger& GetLogger() = 0;
};

}  // namespace pkgmgr
}  // namespace ucm
}  // namespace ara

#endif  // ARA_UCM_PKGMGR_TRANSFER_STREAMABLE_SOFTWARE_PACKAGE_H_
