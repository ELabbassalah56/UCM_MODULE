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

#ifndef ARA_UCM_PKGMGR_PARSING_VERSION_REPORT_IMPL_H
#define ARA_UCM_PKGMGR_PARSING_VERSION_REPORT_IMPL_H

#include "ara/core/string.h"
#include "ara/core/vector.h"
#include "parsing/version_report.h"
#include "parsing/software_cluster_list_builder.h"
#include "ara/log/logging.h"

namespace ara
{
namespace ucm
{
namespace pkgmgr
{

/// @brief Implementation of VersionReport abstract class.
class VersionReportImpl : public VersionReport
{
public:
    /// @brief Implements the reporting of software clusters in different states.
    ///
    /// @param activePartitionSwclListBuilder List of software clusters in active.
    /// @param inactivePartitionSwclListBuilder List of changed software clusters.
    VersionReportImpl(const SoftwareClusterListBuilder& activePartitionSwclListBuilder,
        const SoftwareClusterListBuilder& inactivePartitionSwclListBuilder);

    /// <inheritdoc>
    ara::core::Vector<SwClusterInfoType> GetSwClusterInfo() override;

    /// <inheritdoc>
    ara::core::Vector<SwClusterInfoType> GetSwClusterChangeInfo() override;

protected:
    const SoftwareClusterListBuilder activePartitionSwclListBuilder_;
    const SoftwareClusterListBuilder inactivePartitionSwclListBuilder_;
};

}  // namespace pkgmgr
}  // namespace ucm
}  // namespace ara

#endif  // ARA_UCM_PKGMGR_PARSING_VERSION_REPORT_IMPL_H
