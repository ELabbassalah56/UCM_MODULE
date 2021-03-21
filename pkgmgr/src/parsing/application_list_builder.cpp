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

#include "parsing/application_list_builder.h"

#include "ara/log/logging.h"

namespace ara
{
namespace ucm
{
namespace pkgmgr
{

static ara::log::Logger& logger()
{
    static ara::log::Logger& loggerInstance = ara::log::CreateLogger("PKGM", "PackageManager context");
    return loggerInstance;
}

ApplicationListBuilder::ApplicationListBuilder(Filesystem& fs, const String& searchPath)
    : fs_(fs)
    , searchPath_(searchPath)
{
    if (!searchPath_.empty()) {
        RefreshList();
    }
}

const ara::core::Vector<Application>& ApplicationListBuilder::GetApplications() const
{
    return apps_;
}

int ApplicationListBuilder::GetApplicationCount() const
{
    return apps_.size();
}

void ApplicationListBuilder::RefreshList() noexcept
{
    // drop the old list first
    apps_.clear();

    logger().LogInfo() << "Now scanning through directory" << searchPath_;
    ara::core::Vector<String> directories = fs_.GetSubdirectories(searchPath_);
    for (const auto& dir : directories) {
        String appBinDir(fs_.BuildPath(dir, binDir_));
        String appEtcDir(fs_.BuildPath(dir, etcDir_));
        ara::core::Vector<String> executables = fs_.GetFiles(appBinDir);
        ara::core::Vector<String> manifests = fs_.GetFilesStartingWith(appEtcDir, manifestName_);
        if (executables.empty()) {
            logger().LogError() << "No executable found in " << appBinDir;
        } else {
            for (const auto& exec : executables) {
                if (manifests.size() == 0) {
                    logger().LogError() << "No Manifest file found in " << appEtcDir;
                } else {
                    // loop over all instance configuration files and create application for each
                    for (const auto& manifestPath : manifests) {
                        apps_.emplace_back(dir, exec, fs_.ExtractFileName(exec), manifestPath);
                    }
                }
            }
        }
    }
}

}  // namespace pkgmgr
}  // namespace ucm
}  // namespace ara
