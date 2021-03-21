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

#ifndef _home_user_Desktop_GP_VALEO_AP_ara_project_R19_11_ara_api_ucm_pkgmgr_Build_gen_vsomeip_ara_ucm_pkgmgr_adapter_packagemanagement_h
#define _home_user_Desktop_GP_VALEO_AP_ara_project_R19_11_ara_api_ucm_pkgmgr_Build_gen_vsomeip_ara_ucm_pkgmgr_adapter_packagemanagement_h

#include "ara/com/internal/vsomeip/skeleton/vsomeip_skeleton_base.h"
#include "service_desc_packagemanagement.h"
#include "ara/ucm/pkgmgr/packagemanagement_skeleton.h"

namespace ara {
namespace ucm {
namespace pkgmgr {
namespace packagemanagement_binding {
namespace vsomeip {

class PackageManagementServiceAdapter : public ::ara::com::internal::vsomeip::skeleton::ServiceImplBase {
 public:
  using ServiceInterface = skeleton::PackageManagementSkeleton;
  using ServiceDescriptor = descriptors::internal::Service;

  PackageManagementServiceAdapter(ServiceInterface& service, ara::com::internal::InstanceId instance) :
    ::ara::com::internal::vsomeip::skeleton::ServiceImplBase(service, instance),
    CurrentStatus(instance) {
    Connect(service);
    RegisterMethodDispatcher(ServiceDescriptor::service_id,
                             [this](const std::shared_ptr<::vsomeip::message>& msg) { DispatchMethodCall(msg); });
    OfferService(ServiceDescriptor::service_id, GetInstanceId(), ServiceDescriptor::service_version, ServiceDescriptor::minor_service_version);
    OfferField(ServiceDescriptor::service_id, GetInstanceId(), descriptors::CurrentStatus::event_id, descriptors::CurrentStatus::event_groups);
  }

  virtual ~PackageManagementServiceAdapter() {
    StopOfferField(descriptors::CurrentStatus::service_id, GetInstanceId(), descriptors::CurrentStatus::event_id);
    StopOfferService(ServiceDescriptor::service_id, GetInstanceId());
    UnregisterMethodDispatcher(ServiceDescriptor::service_id);
    Disconnect(dynamic_cast<ServiceInterface&>(service_));
  }
 protected:
  void DispatchMethodCall(const std::shared_ptr<::vsomeip::message>& msg) {
    ServiceInterface& service = dynamic_cast<ServiceInterface&>(service_);
    switch(msg->get_method()) {
      case descriptors::Activate::method_id:
        HandleCall(service, &ServiceInterface::Activate, msg);
        break;
      case descriptors::Cancel::method_id:
        HandleCall(service, &ServiceInterface::Cancel, msg);
        break;
      case descriptors::DeleteTransfer::method_id:
        HandleCall(service, &ServiceInterface::DeleteTransfer, msg);
        break;
      case descriptors::Finish::method_id:
        HandleCall(service, &ServiceInterface::Finish, msg);
        break;
      case descriptors::GetHistory::method_id:
        HandleCall(service, &ServiceInterface::GetHistory, msg);
        break;
      case descriptors::GetLog::method_id:
        HandleCall(service, &ServiceInterface::GetLog, msg);
        break;
      case descriptors::GetSwClusterChangeInfo::method_id:
        HandleCall(service, &ServiceInterface::GetSwClusterChangeInfo, msg);
        break;
      case descriptors::GetSwClusterInfo::method_id:
        HandleCall(service, &ServiceInterface::GetSwClusterInfo, msg);
        break;
      case descriptors::GetSwPackages::method_id:
        HandleCall(service, &ServiceInterface::GetSwPackages, msg);
        break;
      case descriptors::GetSwProcessProgress::method_id:
        HandleCall(service, &ServiceInterface::GetSwProcessProgress, msg);
        break;
      case descriptors::ProcessSwPackage::method_id:
        HandleCall(service, &ServiceInterface::ProcessSwPackage, msg);
        break;
      case descriptors::RevertProcessedSwPackages::method_id:
        HandleCall(service, &ServiceInterface::RevertProcessedSwPackages, msg);
        break;
      case descriptors::Rollback::method_id:
        HandleCall(service, &ServiceInterface::Rollback, msg);
        break;
      case descriptors::SetLogLevel::method_id:
        HandleCall(service, &ServiceInterface::SetLogLevel, msg);
        break;
      case descriptors::TransferData::method_id:
        HandleCall(service, &ServiceInterface::TransferData, msg);
        break;
      case descriptors::TransferExit::method_id:
        HandleCall(service, &ServiceInterface::TransferExit, msg);
        break;
      case descriptors::TransferStart::method_id:
        HandleCall(service, &ServiceInterface::TransferStart, msg);
        break;
    }
  }

 private:
  void Connect(ServiceInterface& service) {
    service.AddDelegate(*this);
    service.CurrentStatus.AddDelegate(CurrentStatus);
  }

  void Disconnect(ServiceInterface& service) {
    service.RemoveDelegate(*this);
    service.CurrentStatus.RemoveDelegate(CurrentStatus);
  }

  ara::com::internal::vsomeip::skeleton::FieldImpl<::ara::ucm::pkgmgr::PackageManagerStatusType, descriptors::CurrentStatus> CurrentStatus;
};

} // namespace vsomeip
} // namespace packagemanagement_binding
} // namespace pkgmgr
} // namespace ucm
} // namespace ara

#endif // _home_user_Desktop_GP_VALEO_AP_ara_project_R19_11_ara_api_ucm_pkgmgr_Build_gen_vsomeip_ara_ucm_pkgmgr_adapter_packagemanagement_h

