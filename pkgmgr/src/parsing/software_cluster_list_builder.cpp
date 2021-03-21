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

#include "software_cluster_list_builder.h"
#include "software_cluster_manifest_parser.h"
#include "software_cluster_manifest.h"

#include <iostream>
#include <fstream>

namespace ara
{
namespace ucm
{
namespace pkgmgr
{

SoftwareClusterListBuilder::SoftwareClusterListBuilder(Filesystem& fs, const String& searchPath)
    : fs_(fs)
    , searchPath_(searchPath)
    , log_(ara::log::CreateLogger("SCLB", "SoftwareClusterListBuilder"))
{
    if (!searchPath_.empty()) {
        RefreshList();
    }
}

const ara::core::Vector<SoftwareCluster>& SoftwareClusterListBuilder::GetSwcls() const
{
    return swcls_;
}

ara::core::Vector<SoftwareCluster>& SoftwareClusterListBuilder::SetSwcls()
{
    return swcls_;
}

int SoftwareClusterListBuilder::GetSwclCount() const
{
    return swcls_.size();
}

void SoftwareClusterListBuilder::RefreshList() noexcept
{
    // drop the old list first
    swcls_.clear();
    boost::property_tree::ptree manifestTree;
    SoftwareClusterManifestParser parser_;

    log_.LogInfo() << "Now scanning through directory " << searchPath_;

    ara::core::Vector<String> manifestsPaths = fs_.GetFilesRecursive(searchPath_);

    if (manifestsPaths.empty()) {
        log_.LogError() << "No swcl manifest found in " << searchPath_;
    } else {

        for (const auto& manifestPath : manifestsPaths) {
            if (manifestPath.find(manifestName_) != ara::core::String::npos) {
                try {
                    swcls_.emplace_back(*parser_.ParseFromFile(manifestPath), manifestPath);
                } catch (exception::InvalidManifestException& e) {
                    log_.LogError() << "Could not parse manifest" << e.what();
                }
            }
        }
    }
}

}  // namespace pkgmgr
}  // namespace ucm
}  // namespace ara
