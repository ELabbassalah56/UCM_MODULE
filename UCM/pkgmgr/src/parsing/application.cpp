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

#include "parsing/application.h"

#include "ara/log/logging.h"
#include "ara/per/key_value_storage.h"
#include "ara/per/kvstype/kvs_type.h"

using ara::per::KeyValueStorage;
using ara::per::kvstype::KvsType;

namespace ara
{
namespace ucm
{
namespace pkgmgr
{

static ara::log::Logger& logger()
{
    static ara::log::Logger& loggerInstance = ara::log::CreateLogger("APPL", "Application context");
    return loggerInstance;
}

namespace
{
static const String kStartupConfig = "startup_configs";
static const String kDepends = "depends";
}  // namespace

Application::Application(const String& prefix,
    const String& executable,
    const String& applicationName,
    const String& manifestPath,
    const Version& version)
    : prefix_(prefix)
    , executable_(executable)
    , applicationName_(applicationName)
    , manifestPath_(manifestPath)
    , version_(version)
{
    ParseApplicationManifest();
}

const ara::core::Vector<String>& Application::GetDependencies() const
{
    return dependencies_;
}

const String& Application::GetExecutable() const noexcept
{
    return executable_;
}

const String& Application::GetApplicationName() const noexcept
{
    return applicationName_;
}

const String& Application::GetManifestPath() const noexcept
{
    return manifestPath_;
}

const String& Application::GetApplicationPrefix() const noexcept
{
    return prefix_;
}

const Version& Application::GetApplicationVersion() const noexcept
{
    return version_;
}

void Application::ParseApplicationManifest()
{
    if (manifestPath_.empty()) {
        return;
    }

    auto storage = ara::per::OpenKeyValueStorage(manifestPath_);
    if (storage) {
        dependencies_ = GetApplicationDependencies(std::move(storage).Value());
    } else {
        logger().LogError() << "Failed to load data for application " << applicationName_
                            << " from Manifest file at path " << manifestPath_;
        logger().LogError() << storage.Error().Message();
    }
}

ara::core::Vector<String> Application::GetApplicationDependencies(
    ara::per::SharedHandle<ara::per::KeyValueStorage> manifestStorage)

{
    KvsType kvstmp;
    auto status = manifestStorage->GetValue(kStartupConfig, kvstmp);

    if (!status) {
        logger().LogError() << "Error while parsing application manifest for application " << applicationName_
                            << " at path " << manifestPath_ << ". Error: " << status.Error().Message();
        return {};                  
    }

    auto startupConfigs = kvstmp.GetArray<KvsType>();
    if (!startupConfigs) {
        logger().LogError() << "Error while parsing application manifest for application " << applicationName_
                            << " at path " << manifestPath_ << ". Failed to get startup configs.";
        return {};
    }

    return GetApplicationDependenciesForStartupConfigs(std::move(startupConfigs).Value());
}

ara::core::Vector<String> Application::GetApplicationDependenciesForStartupConfigs(
    ara::core::Vector<KvsType>&& startupConfigs)
{
    if (startupConfigs.empty()) {
        logger().LogError() << "Error while parsing application manifest for application " << applicationName_
                            << " at path " << manifestPath_ << ". Manifest has no startup config";
        return {};
    }

    ara::core::Vector<String> dependencies;
    for (KvsType& config : startupConfigs) {
        auto params = config.GetArray<KvsType>();
        if (!params) {
            logger().LogError() << "Error while parsing application manifest for application " << applicationName_
                                << " at path " << manifestPath_ << ". Failed to read parameters from startup config.";
            return {};
        }

        ara::core::Vector<KvsType> startupParams = std::move(params).Value();
        for (KvsType& param : startupParams) {
            if (kDepends == param.GetKey()) {
                auto value = param.GetArray<ara::core::String>();
                if (!value) {
                    logger().LogError() << "Error while parsing application manifest for application "
                                        << applicationName_ << " at path " << manifestPath_
                                        << ". Failed to read dependencies from startup config.";
                    return {};
                }

                ara::core::Vector<ara::core::String> stateDependencies = std::move(value).Value();
                dependencies.insert(dependencies.end(), stateDependencies.begin(), stateDependencies.end());
            }
        }
    }
    return dependencies;
}

}  // namespace pkgmgr
}  // namespace ucm
}  // namespace ara
