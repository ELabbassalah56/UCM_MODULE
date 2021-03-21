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

#ifndef ARA_UCM_PKGMGR_PARSING_APPLICATION_H_
#define ARA_UCM_PKGMGR_PARSING_APPLICATION_H_

#include "ara/core/string.h"
#include "ara/core/vector.h"

#include "parsing/version.h"
#include "ara/per/key_value_storage.h"
#include "ara/per/kvstype/kvs_type.h"

namespace ara
{
namespace ucm
{
namespace pkgmgr
{
using String = ara::core::String;
/// @brief This class represents an adaptive application
/// This class has been adapted from the Execution Manager
///
/// @uptrace{SWS_UCM_00004}
class Application
{
public:
    /// @brief Initialzes a new Application
    ///
    /// @param prefix The prefix of the executable path
    /// @param executable The name of the executable file for this application
    /// @param applicationName The name of this application
    /// @param manifestPath The absolte path to the manifest file
    /// @param version Explicitly defined version for this application
    ///   This version parameter is only used for testing because the application manifest
    ///   does currently not support a version
    Application(const String& prefix,
        const String& executable,
        const String& applicationName,
        const String& manifestPath,
        const Version& version = Version(1, 0));

    virtual ~Application() = default;

    /// @brief Returns the dependencies of this application as mentioned in the application manifest
    ///
    /// @returns List of application names
    virtual const ara::core::Vector<String>& GetDependencies() const;

    /// @brief Returns the absolute path to the executable of this application
    ///
    /// @returns Absolute path to executable
    const String& GetExecutable() const noexcept;

    /// @brief Returns the name of this application
    ///
    /// @returns application name
    virtual const String& GetApplicationName() const noexcept;

    /// @brief Returns the absolute path to this applications base directory
    ///
    /// @returns absolute path to base directory (e.g. /opt/DiagnosticManager)
    const String& GetApplicationPrefix() const noexcept;

    /// @brief Returns the version of this  application
    ///
    /// @returns application version
    virtual const Version& GetApplicationVersion() const noexcept;

    /// @brief Returns the absolute path to this applications application manfiest file
    ///
    /// @returns absolute path to manifest
    const String& GetManifestPath() const noexcept;

protected:
    /// @brief Try to parse the application manifest file and initialize the list of dependencies.
    /// The path to this file has been specified in the constructor.
    virtual void ParseApplicationManifest();

    /// @brief Extracts the application dependencies from the manifest file
    ///
    /// @param manifestStorage The parsed manifest file
    ///
    /// @returns The list of application dependencies from all startup_configs combined

    virtual ara::core::Vector<String> GetApplicationDependencies(
        ara::per::SharedHandle<ara::per::KeyValueStorage> manifestStorage);
    // virtual ara::core::Vector<String> GetApplicationDependencies(
    //     ara::per::KeyValueStorageHandle manifestStorage);

    /// @brief Extracts the application dependencies from the given startup configs
    ///
    /// @param startupConfigs The startup configs that are searched for runtime dependencies
    ///
    /// @returns The list of application dependencies from all given startup configs combined
    virtual ara::core::Vector<String> GetApplicationDependenciesForStartupConfigs(
        ara::core::Vector<ara::per::kvstype::KvsType>&& startupConfigs);

    /// @brief List of dependencies for this application
    /// @note This list is filled in the constructor
    ara::core::Vector<String> dependencies_;

private:
    /// @brief Absolute path to this applications base directory
    String prefix_;

    /// @brief Absolute path to this applications executable
    String executable_;

    /// @brief Name of this application
    String applicationName_;

    /// @brief Absolute path to this applications manifest file
    String manifestPath_;

    /// @brief The version of this application
    Version version_;
};

}  // namespace pkgmgr
}  // namespace ucm
}  // namespace ara

#endif  // ARA_UCM_PKGMGR_PARSING_APPLICATION_H_
