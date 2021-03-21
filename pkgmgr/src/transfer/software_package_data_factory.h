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

#ifndef ARA_UCM_PKGMGR_TRANSFER_SOFTWARE_PACKAGE_DATA_FACTORY_H_
#define ARA_UCM_PKGMGR_TRANSFER_SOFTWARE_PACKAGE_DATA_FACTORY_H_

#include "streamable_software_package_factory.h"
#include "software_package_data.h"
#include "ara/core/string.h"

namespace ara
{
namespace ucm
{
namespace pkgmgr
{

/// @brief An interface for creating SoftwarePackageData implementations of
/// StreamableSoftwarePackage interface.
///
/// The implementations created by this factory are used to parametrize
/// PackageManager service implementations.
///
class SoftwarePackageDataFactory : public StreamableSoftwarePackageFactory
{
public:
    ~SoftwarePackageDataFactory() = default;

    std::unique_ptr<StreamableSoftwarePackage> Create(TransferIdType id) override
    {
        return std::make_unique<SoftwarePackageData>(id);
    }

private:
};

}  // namespace pkgmgr
}  // namespace ucm
}  // namespace ara

#endif  // ARA_UCM_PKGMGR_TRANSFER_SOFTWARE_PACKAGE_DATA_FACTORY_H_
