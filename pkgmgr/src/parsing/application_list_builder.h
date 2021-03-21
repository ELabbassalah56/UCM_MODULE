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

/// @file
/// This file has been mostly copied from exec/ExecutionManager/src/application_list_builder.h
/// This file should become obsolete once the EM offers additional interface to retrieve a list of
/// currently installed applications

#ifndef ARA_UCM_PKGMGR_PARSING_APPLICATION_LIST_BUILDER_H_
#define ARA_UCM_PKGMGR_PARSING_APPLICATION_LIST_BUILDER_H_

#include "ara/core/vector.h"
#include "filesystem.h"
#include "parsing/application.h"

namespace ara
{
namespace ucm
{
namespace pkgmgr
{
using String = ara::core::String;
/// @brief The ApplicationListBuilder is used to scan the installation directory and
/// assemble a list of installed applications
///
/// @uptrace{SWS_UCM_00004}
class ApplicationListBuilder
{
public:
    /// @brief Initializes an ApplicationListBuilder with a given path to search for
    /// adaptive applications
    ///
    /// @param fs To access the filesystem
    /// @param searchPath The path to search for adaptive applications
    ApplicationListBuilder(Filesystem& fs, const String& searchPath);

    virtual ~ApplicationListBuilder() = default;

    /// @brief Returns a list of application that have been found in the searchpath
    ///
    /// @returns Applications in searchpath
    virtual const ara::core::Vector<Application>& GetApplications() const;

    /// @brief Returns the number of adaptive applications that have been found in the searchpath
    ///
    /// @returns Number of applications in searchpath
    virtual int GetApplicationCount() const;

    /// @brief Scans the searchpath again for adaptive applications
    /// Does only need to be called in case new software was installed after this application
    /// list builder object has been initialized
    virtual void RefreshList() noexcept;

private:
    /// @brief The filesystem class is used to access the searchpath directory and its content
    Filesystem& fs_;

    /// @brief This directory is searched for applications
    const String searchPath_;

    /// @brief This list holds the applications found in searchpath
    /// @note This list can be updated using RefreshList
    ara::core::Vector<Application> apps_;

    /// @brief The bin directory of an application folder which contains the executable
    const String binDir_ = "bin";

    /// @brief The etc directory of an application folder which contains the manifest
    const String etcDir_ = "etc";

    /// @brief The name of the manifest file
    const String manifestName_ = "MANIFEST";
};

}  // namespace pkgmgr
}  // namespace ucm
}  // namespace ara

#endif  // ARA_UCM_PKGMGR_PARSING_APPLICATION_LIST_BUILDER_H_
