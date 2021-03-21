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

#ifndef _home_user_Desktop_GP_VALEO_AP_ara_project_R19_11_ara_api_ucm_pkgmgr_Build_gen_vsomeip_vsomeip_service_mapping_package_manager_swc_cpp
#define _home_user_Desktop_GP_VALEO_AP_ara_project_R19_11_ara_api_ucm_pkgmgr_Build_gen_vsomeip_vsomeip_service_mapping_package_manager_swc_cpp

#include "ara/com/internal/vsomeip/vsomeip_service_mapping.h"
#include "ara/com/internal/vsomeip/vsomeip_service_mapping_impl.h"

#include "ara/ucm/pkgmgr/adapter_packagemanagement.h"

namespace ara {
namespace com {
namespace internal {
namespace vsomeip {
namespace runtime {

namespace {

ServiceMappingImpl<
  ara::ucm::pkgmgr::PackageManagement::service_id,
  ara::ucm::pkgmgr::packagemanagement_binding::vsomeip::descriptors::internal::Service::service_id,
  NoProxy,
  ara::ucm::pkgmgr::packagemanagement_binding::vsomeip::PackageManagementServiceAdapter
> ara__ucm__pkgmgr__packagemanagement__mapping;

}

const VSomeIPServiceMapping::Mapping* VSomeIPServiceMapping::GetMappingForServiceId(ServiceId service_id) {
  switch (service_id) {
    case ara::ucm::pkgmgr::PackageManagement::service_id:
      return &ara__ucm__pkgmgr__packagemanagement__mapping;
    default:
      return nullptr;
  }
}

VSomeIPServiceMapping::MultiMapping VSomeIPServiceMapping::GetMappingForVSomeIPServiceId(::vsomeip::service_t service_id) {
  switch(service_id) {
    case ara::ucm::pkgmgr::packagemanagement_binding::vsomeip::descriptors::internal::Service::service_id:
      return {
        &ara__ucm__pkgmgr__packagemanagement__mapping,
      };
    default:
      return {};
  }
}

} // namespace runtime
} // namespace vsomeip
} // namespace internal
} // namespace com
} // namespace ara

#endif // _home_user_Desktop_GP_VALEO_AP_ara_project_R19_11_ara_api_ucm_pkgmgr_Build_gen_vsomeip_vsomeip_service_mapping_package_manager_swc_cpp

