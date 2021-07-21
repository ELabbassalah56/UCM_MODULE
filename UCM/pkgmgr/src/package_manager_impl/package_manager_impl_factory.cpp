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

#include "package_manager_impl/package_manager_impl_factory.h"
#include "package_manager_impl/package_manager_impl.h"
#include "transfer/software_package_data_factory.h"
#include "extraction/lib_poco_zip_extractor.h"
#include "storage/filesystem_swcl_manager.h"
#include "storage/processes_list_manager.h"

namespace ara
{
namespace ucm
{
namespace pkgmgr
{

PackageManagerImplFactory::PackageManagerImplFactory(const core::String& installDir)
    : installDirectory_(installDir)
{
      std::cout<<installDir<<"\t"<<"elabbas.salah"<<std::endl; 
}

std::unique_ptr<PackageManagerImpl> PackageManagerImplFactory::Create()
{

    return std::make_unique<PackageManagerImpl>(                                                           // Package Manager
                      std::make_unique<LibPocoZipExtractor>(fs_),                                                        // SoftwarePackageExtractor
                      std::make_unique<SoftwarePackageParserImpl>(fs_, swclManifestParser_, swPackageManifestParser_),   // SoftwarePackageParser
                      std::make_unique<FileSystemSWCLManager>(installDirectory_),                                        // SWCLManager
                      std::make_unique<ProcessesListManager>(installDirectory_),                                         // Processes List Manager
                      std::make_unique<SoftwarePackageDataFactory>(),                                                    // StreamableSoftwarePackageFactory
                      std::make_unique<SynchronizedStorage<TransferIdType, StreamableSoftwarePackage>>(),                // packagesData
                      installDirectory_);
}

}  // namespace pkgmgr
}  // namespace ucm
}  // namespace ara
