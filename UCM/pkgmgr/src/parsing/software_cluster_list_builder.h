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

#ifndef ARA_UCM_PKGMGR_SOFTWARE_CLUSTER_LIST_BUILDER_H_
#define ARA_UCM_PKGMGR_SOFTWARE_CLUSTER_LIST_BUILDER_H_

#include "ara/core/string.h"
#include "ara/core/vector.h"
#include "filesystem.h"
#include "software_cluster.h"
#include "software_cluster_manifest_parser.h"

namespace ara
{
namespace ucm
{
namespace pkgmgr
{
using String = ara::core::String;
/// @brief The SoftwareClusterListBuilder is used to scan the UCM swcl directory and
/// assemble a list of installed SwCls
///
/// @uptrace{SWS_UCM_00004}
class SoftwareClusterListBuilder
{
public:
    /// @brief Initializes an SoftwareClusterListBuilder with a given path to search for
    /// adaptive swcls
    ///
    /// @param fs To access the filesystem
    /// @param searchPath The path to search for swcls
    SoftwareClusterListBuilder(Filesystem& fs, const String& searchPath);

    virtual ~SoftwareClusterListBuilder() = default;

    /// @brief Returns a list of Software Clusters that have been found in the searchpath
    ///
    /// @return Swcls in searchpath
    virtual const ara::core::Vector<SoftwareCluster>& GetSwcls() const;

    /// @brief Returns a modifiable list of Software Clusters that have been found in the searchpath
    ///
    /// @return modifiable Swcls in searchpath
    virtual ara::core::Vector<SoftwareCluster>& SetSwcls();

    /// @brief Returns the number of swcls that have been found in the searchpath
    ///
    /// @return Number of swcls in searchpath
    virtual int GetSwclCount() const;

    /// @brief Scans the searchpath again for swcls
    /// Does only need to be called in case new swcl was installed after this swcl
    /// list builder object has been initialized
    virtual void RefreshList() noexcept;

protected:
    /// @brief This list holds the swcls found in searchpath
    /// @note This list can be updated using RefreshList
    ara::core::Vector<SoftwareCluster> swcls_;

private:
    /// @brief The filesystem class is used to access the searchpath directory and its content
    Filesystem& fs_;

    /// @brief This directory is searched for swcls
    const String searchPath_;

    /// @brief Logger for logging errors
    ara::log::Logger& log_;

    /// @brief The name of the manifest file
    const String manifestName_ = "SWCL_MANIFEST";
};

}  // namespace pkgmgr
}  // namespace ucm
}  // namespace ara

#endif  // ARA_UCM_PKGMGR_SOFTWARE_CLUSTER_LIST_BUILDER_H_
