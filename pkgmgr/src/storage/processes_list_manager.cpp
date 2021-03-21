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

#include "processes_list_manager.h"
#include "filesystem_swcl_manager.h"
#include "parsing/application_list_builder.h"
#include "ara/per/key_value_storage.h"

namespace ara
{
namespace ucm
{
namespace pkgmgr
{

ProcessesListManager::ProcessesListManager(ara::core::String rootDirectory)
    : rootDirectory_(rootDirectory)
{}

void ProcessesListManager::UpdateProcessesList(ara::core::Vector<SoftwareCluster> activeSWCLs)
{
    Filesystem fs;
    ApplicationListBuilder builder(fs, rootDirectory_);
    ara::core::Vector<Application> apps = builder.GetApplications();

    for (const auto& app : apps) {
        appsPaths.push_back(app.GetApplicationPrefix());
    }

    ara::core::Vector<String> swclsProcessesList;

    for (auto& cluster : activeSWCLs) {
        auto swclManifestPath = cluster.GetManifestPath();
        ara::core::Vector<String> swclApps = fs.GetSubdirectories(fs.GetFilePrefix(swclManifestPath));
        for (auto& swclApp : swclApps) {
            ara::core::Vector<String> swclAppProcesses = fs.GetSubdirectories(swclApp);
            swclsProcessesList.insert(
                std::end(swclsProcessesList), std::begin(swclAppProcesses), std::end(swclAppProcesses));
        }
    }

    appsPaths.insert(std::end(appsPaths), std::begin(swclsProcessesList), std::end(swclsProcessesList));

    log.LogInfo() << "Processes list is being updated: " << (rootDirectory_ + kProcessesListFileName).c_str();

    auto kvs = ara::per::OpenKeyValueStorage((rootDirectory_ + kProcessesListFileName).c_str());

    if (!kvs) {
        log.LogInfo() << "Processes list update failed";
    } else {
        auto db = std::move(kvs).Value();

        auto result = db->SetValue(kKeyName, appsPaths);
        db->SyncToStorage();

        log.LogInfo() << "Processes list is successfully updated";
    }
}

}  // namespace pkgmgr
}  // namespace ucm
}  // namespace ara
