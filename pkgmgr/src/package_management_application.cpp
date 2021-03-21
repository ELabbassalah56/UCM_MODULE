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

#include "package_management_application.h"
#include "package_manager_impl/package_manager_impl_factory.h"
#include "package_manager/package_manager.h"
#include "package_manager_state/package_manager_idle_state.h"
#include "ara/core/string.h"
#include "boost/property_tree/ptree.hpp"
#include "boost/property_tree/json_parser.hpp"

namespace ara
{
namespace ucm
{
namespace pkgmgr
{

// a directory where applications are installed
const core::String kInstallDirectory{"/var/lib/apd_ucm"};

PackageManagementApplication::PackageManagementApplication()
    : log_(ara::log::CreateLogger("UCMR", "UCM Runtime context", ara::log::LogLevel::kVerbose)){}
bool PackageManagementApplication::OnInitialize()
{
    // <SERVICE-INSTANCE-ID>828</SERVICE-INSTANCE-ID>
    // std::uint16_t type is used as internal::ServiceInstanceId to create InstanceIdentifier
    // It is not currently possible to create InstanceIdentifier from string
    // This ServiceInstanceId is extracted from the generated Service Interface Manifest file

    uint16_t serviceInstanceId;

    try {
        serviceInstanceId = GetServiceInstanceId(serviceDeploymentManifestPath_);
    } catch (const exception::InvalidManifestException& e) {
         std::cout<<"faild id \n"<<std::endl;  /*@Abbas-Salah-gdb*/
        return false;
    }

    // com::InstanceIdentifier serviceInstanceId( id );
    // install directory should be read from the config file
    PackageManagerImplFactory implFactory{kInstallDirectory};
    //start /*@Abbas-Salah-gdb*/
    //create instance from package managment implementaion commuincation with internal 
    //provider service  
    //then  create instance pointer commuincate with out world through ara::com
    //end /*@Abbas-Salah-gdb*/ 
    service_ = std::make_unique<PackageManager>(implFactory.Create(), serviceInstanceId);
    log_.LogInfo() << "Package Management service instance is created with id " << serviceInstanceId<<"\n";
    return true;
}

void PackageManagementApplication::Run()
{
    log_.LogInfo() << "Adaptive AUTOSAR Update And Configuration Management running";
    // start offering service here
    service_->OfferService();
    // report that execution state is running state
    ReportRunningState();
    WaitUntilTermination();
}

void PackageManagementApplication::OnTerminate()
{
    log_.LogInfo() << "Adaptive AUTOSAR Update And Configuration Management terminating";
}

uint16_t PackageManagementApplication::GetServiceInstanceId(const ara::core::String& ManifestPath)
{
    boost::property_tree::ptree serviceDeploymentManifestTree;
    // read the Service Interface Manifest file and store its data into a Ptree
    try {
        boost::property_tree::read_json(ManifestPath.c_str(), serviceDeploymentManifestTree);
    } catch (const boost::property_tree::file_parser_error& error) {
        ara::core::String msg = "Error reading manifest from filesystem at " + ManifestPath + " :" + error.what();
        log_.LogError() << msg;
        throw exception::InvalidManifestException(msg);
    }

    // scan the Ptree and find the value associated with the key "instance"
    std::string serviceInstanceIdString;

    for (boost::property_tree::ptree::value_type& val : serviceDeploymentManifestTree.get_child("services")) {
        serviceInstanceIdString = val.second.get<std::string>("instance");

        std::cout<<serviceInstanceIdString<<"  "<<&val<<std::endl; ///*@Abbas-Salah-gdb*/ done service 828 is could
    }

    // convert string to an hexadecimal uint16_t
    std::stringstream str;
    str << serviceInstanceIdString;
    std::uint16_t serviceInstanceId;
    str >> std::hex >> serviceInstanceId;

    return serviceInstanceId;
}

}  // namespace pkgmgr
}  // namespace ucm
}  // namespace ara
