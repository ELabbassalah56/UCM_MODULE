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

#ifndef _home_user_Desktop_GP_VALEO_AP_ara_project_R19_11_ara_api_ucm_pkgmgr_Build_gen_vsomeip_ara_ucm_pkgmgr_service_desc_packagemanagement_h
#define _home_user_Desktop_GP_VALEO_AP_ara_project_R19_11_ara_api_ucm_pkgmgr_Build_gen_vsomeip_ara_ucm_pkgmgr_service_desc_packagemanagement_h

#include "ara/ucm/pkgmgr/packagemanagement_common.h"

#include "ara/com/internal/vsomeip/vsomeip_types.h"

namespace ara {
namespace ucm {
namespace pkgmgr {
namespace packagemanagement_binding {
namespace vsomeip {
namespace descriptors {


namespace internal {
    struct Service {
        static constexpr ara::com::internal::vsomeip::types::ServiceId service_id = 0x275;
        static constexpr ara::com::internal::vsomeip::types::ServiceVersionMajor service_version = 0x1;
        static constexpr ara::com::internal::vsomeip::types::ServiceVersionMinor minor_service_version = 0x0;
    };
}

struct Activate: public internal::Service {
  static constexpr ara::com::internal::vsomeip::types::MethodId method_id = 0x75bc;
  static constexpr bool is_reliable = false;
};

struct Cancel: public internal::Service {
  static constexpr ara::com::internal::vsomeip::types::MethodId method_id = 0x75a8;
  static constexpr bool is_reliable = false;
};

struct DeleteTransfer: public internal::Service {
  static constexpr ara::com::internal::vsomeip::types::MethodId method_id = 0x7580;
  static constexpr bool is_reliable = false;
};

struct Finish: public internal::Service {
  static constexpr ara::com::internal::vsomeip::types::MethodId method_id = 0x75c6;
  static constexpr bool is_reliable = false;
};

struct GetHistory: public internal::Service {
  static constexpr ara::com::internal::vsomeip::types::MethodId method_id = 0x75f8;
  static constexpr bool is_reliable = false;
};

struct GetLog: public internal::Service {
  static constexpr ara::com::internal::vsomeip::types::MethodId method_id = 0x75da;
  static constexpr bool is_reliable = false;
};

struct GetSwClusterChangeInfo: public internal::Service {
  static constexpr ara::com::internal::vsomeip::types::MethodId method_id = 0x7530;
  static constexpr bool is_reliable = false;
};

struct GetSwClusterInfo: public internal::Service {
  static constexpr ara::com::internal::vsomeip::types::MethodId method_id = 0x74cc;
  static constexpr bool is_reliable = false;
};

struct GetSwPackages: public internal::Service {
  static constexpr ara::com::internal::vsomeip::types::MethodId method_id = 0x7544;
  static constexpr bool is_reliable = false;
};

struct GetSwProcessProgress: public internal::Service {
  static constexpr ara::com::internal::vsomeip::types::MethodId method_id = 0x759e;
  static constexpr bool is_reliable = false;
};

struct ProcessSwPackage: public internal::Service {
  static constexpr ara::com::internal::vsomeip::types::MethodId method_id = 0x758a;
  static constexpr bool is_reliable = false;
};

struct RevertProcessedSwPackages: public internal::Service {
  static constexpr ara::com::internal::vsomeip::types::MethodId method_id = 0x7594;
  static constexpr bool is_reliable = false;
};

struct Rollback: public internal::Service {
  static constexpr ara::com::internal::vsomeip::types::MethodId method_id = 0x75b2;
  static constexpr bool is_reliable = false;
};

struct SetLogLevel: public internal::Service {
  static constexpr ara::com::internal::vsomeip::types::MethodId method_id = 0x75d0;
  static constexpr bool is_reliable = false;
};

struct TransferData: public internal::Service {
  static constexpr ara::com::internal::vsomeip::types::MethodId method_id = 0x756c;
  static constexpr bool is_reliable = false;
};

struct TransferExit: public internal::Service {
  static constexpr ara::com::internal::vsomeip::types::MethodId method_id = 0x7576;
  static constexpr bool is_reliable = false;
};

struct TransferStart: public internal::Service {
  static constexpr ara::com::internal::vsomeip::types::MethodId method_id = 0x7562;
  static constexpr bool is_reliable = false;
};

struct CurrentStatus : public internal::Service {
    static constexpr ara::com::internal::vsomeip::types::EventId event_id = 0xa0f6;
    static constexpr ara::com::internal::vsomeip::types::MethodId get_method_id = 0x7535;
    static constexpr bool is_getter_reliable = false;
    static constexpr ara::com::internal::vsomeip::types::EventGroupId event_groups[] { 0xad74 };
};
} // namespace descriptors
} // namespace vsomeip
} // namespace packagemanagement_binding
} // namespace pkgmgr
} // namespace ucm
} // namespace ara

#endif // _home_user_Desktop_GP_VALEO_AP_ara_project_R19_11_ara_api_ucm_pkgmgr_Build_gen_vsomeip_ara_ucm_pkgmgr_service_desc_packagemanagement_h

