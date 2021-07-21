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

#include "parsing/version_report_impl.h"
#include "parsing/software_cluster.h"
#include "ara/log/logging.h"
#include <sstream>

namespace ara
{
namespace ucm
{
namespace pkgmgr
{

VersionReportImpl::VersionReportImpl(const SoftwareClusterListBuilder& activePartitionSwclListBuilder,
    const SoftwareClusterListBuilder& inactivePartitionSwclListBuilder)
    : activePartitionSwclListBuilder_(activePartitionSwclListBuilder)
    , inactivePartitionSwclListBuilder_(inactivePartitionSwclListBuilder)
{}

ara::core::Vector<SwClusterInfoType> VersionReportImpl::GetSwClusterInfo()
{

    ara::core::Vector<SwClusterInfoType> presentSwclListInfo;
    ara::core::Vector<SoftwareCluster> activePartitionSwclListInfo = activePartitionSwclListBuilder_.GetSwcls();

    for (auto const& activeSwclListItem : activePartitionSwclListInfo) {
        presentSwclListInfo.push_back({activeSwclListItem.GetSwclManifest().GetShortName(),
            activeSwclListItem.GetSwclManifest().GetVersion().ToString(),
            SwClusterStateType::kPresent});
    }

    return presentSwclListInfo;
}

ara::core::Vector<SwClusterInfoType> VersionReportImpl::GetSwClusterChangeInfo()
{
    ara::core::Vector<SwClusterInfoType> changeSwclListInfo;
    ara::core::Vector<SoftwareCluster> inactivePartitionSwclListInfo = inactivePartitionSwclListBuilder_.GetSwcls();

    for (auto const& inactiveSwclListItem : inactivePartitionSwclListInfo) {
        if (inactiveSwclListItem.GetSwclState() != SwClusterStateType::kPresent) {
            changeSwclListInfo.push_back({inactiveSwclListItem.GetSwclManifest().GetShortName(),
                inactiveSwclListItem.GetSwclManifest().GetVersion().ToString(),
                inactiveSwclListItem.GetSwclState()});
        }
    }

    return changeSwclListInfo;
}

}  // namespace pkgmgr
}  // namespace ucm
}  // namespace ara
