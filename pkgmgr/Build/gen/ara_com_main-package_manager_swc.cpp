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

#ifndef _home_user_Desktop_GP_VALEO_AP_ara_project_R19_11_ara_api_ucm_pkgmgr_Build_gen_ara_com_main_package_manager_swc_cpp
#define _home_user_Desktop_GP_VALEO_AP_ara_project_R19_11_ara_api_ucm_pkgmgr_Build_gen_ara_com_main_package_manager_swc_cpp

#include "ara/com/internal/ara_com_main.h"

#ifdef HAS_VSOMEIP_BINDING
#include "ara/com/internal/vsomeip/vsomeip_binding.h"
#include "ara/com/internal/vsomeip/vsomeip_error_domains.h"
#include "vsomeip/ara/ucm/pkgmgr/errors_packagemanagement.h"
#include "error_domain_ucmerrordomain.h"
#endif // HAS_VSOMEIP_BINDING


#ifdef HAS_PROLOC_BINDING
#include "ara/com/internal/proloc/proloc_binding.h"
#endif // HAS_PROLOC_BINDING

namespace ara {
namespace com {
namespace internal {
namespace runtime {

void Initialize() {
#ifdef HAS_VSOMEIP_BINDING
    vsomeip::runtime::Register();
    ara::ucm::pkgmgr::packagemanagement_binding::vsomeip::registerErrors();
    ara::com::internal::vsomeip::common::error_domains::Registry::registerDomain(ara::ucm::pkgmgr::GetUCMErrorDomainErrorDomain());
#endif // HAS_VSOMEIP_BINDING

#ifdef HAS_PROLOC_BINDING
    proloc::runtime::Register();
#endif // HAS_PROLOC_BINDING
}

} // namespace runtime
} // namespace internal
} // namespace com
} // namespace ara


#endif // _home_user_Desktop_GP_VALEO_AP_ara_project_R19_11_ara_api_ucm_pkgmgr_Build_gen_ara_com_main_package_manager_swc_cpp

