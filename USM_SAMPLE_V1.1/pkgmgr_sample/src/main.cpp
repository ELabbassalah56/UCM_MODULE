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

#include "ara/core/string.h"
#include "ara/com/internal/runtime.h"
#include "ara/exec/execution_client.h"
#include "ara/log/logging.h"
#include "ara/log/logmanager.h"
#include "find_service.h"
#include "package_management_app.h"
#include <chrono>
#include <memory>
#include <condition_variable>

using ara::ucm::pkgmgr::proxy::PackageManagementProxy;
using ara::ucm::pkgmgrsample::PackageManagementApp;

inline void trace(const ara::core::String &str)
{
    std::cout << "pkgApp: " << str << std::endl;
}

void ReportIntegrationTestResult(bool result)
{
    auto &logger = ara::log::CreateLogger("RES", "RESULT context", ara::log::LogLevel::kVerbose);

    logger.LogInfo() << "Reporting Integration Test Result";
    if (result)
    {
        trace("INTEGRATION TEST <TC_PKGMGR_PackageManagement> {SUCCESS}");
    }
    else
    {
        trace("INTEGRATION TEST <TC_PKGMGR_PackageManagement> {FAIL}");
    }
}

void PackageManagerTestSequence()
{
    const ara::core::String testDataLocation = "/usr/share/apdtest/ucm/";
    const std::vector<ara::core::String> packages = {
                                                          testDataLocation + "SWP_TEST1_INS.zip",
                                                        //  testDataLocation + "SWP_TEST1_REM.zip",
                                                        //  testDataLocation + "SWP_TEST1_INS (copy).zip",
                                                        //  testDataLocation + "SWP_TEST1_INS (another copy).zip",
                                                        //  testDataLocation + "SWP_TEST1_UPD.zip"
                                                    };
    bool integrationTestResult = false;

    auto foundService = ara::ucm::pkgmgrsample::FindPackageManagementService();
    ara::ucm::pkgmgrsample::ServiceProxyPtr service = nullptr;
    if (foundService)
    {
        std::cout << "foundService" << std::endl;
        service = *foundService;
    }
    else
    {

        std::cout << "Don't Found Service" << std::endl;
        ReportIntegrationTestResult(false);
        return;
    }

    PackageManagementApp<PackageManagementProxy> app;
    
    std::cout << "Package Managemnt App" << std::endl;

    auto &logger = ara::log::CreateLogger("TEST", "Test Sequence context", ara::log::LogLevel::kVerbose);

    logger.LogInfo() << "Created PackageManagementApp";

    std::unique_lock<std::mutex> lck(app.globalMutex);

    if (app.Init(service) && app.UpdateStatus())
    {
        integrationTestResult = app.cond.wait_for(lck, std::chrono::milliseconds(10000), [&] {
            std::cout << "Update State to kIdle" << std::endl;
            return app.IsCorrectStatus(ara::ucm::pkgmgr::PackageManagerStatusType::kIdle);
        });

        if ((!integrationTestResult) || app.AreChangesPending())
        {
            std::cout<<"Recovery application"<<std::endl;
            
            integrationTestResult = app.AttemptStateRecovery();
            if (integrationTestResult)
            {
                logger.LogInfo() << "State Recovery Successful ";
            }
            else
            {
                logger.LogInfo() << "State Recovery Failed ";
            }
        }

        if (integrationTestResult)
        {
            for (const ara::core::String &package : packages)
            {
                std::cout<<"Transfer Package"<<std::endl;
                
                integrationTestResult = app.TransferSoftwarePackage(package);
                if (integrationTestResult)
                {
                 std::cout<<"Install Applcations"<<std::endl;

                    integrationTestResult = app.Install();

                    if (!integrationTestResult)
                    {
                        break;
                    }
                    integrationTestResult = app.cond.wait_for(lck, std::chrono::milliseconds(10000), [&] {
                        return app.IsCorrectStatus(ara::ucm::pkgmgr::PackageManagerStatusType::kIdle);
                    });
                    if (!integrationTestResult)
                    {
                        break;
                    }
                }
                else
                {
                    break;
                }
            }
            integrationTestResult = app.DeleteSoftwarePackages();
        }
    }
     

    if (integrationTestResult)
    {

        std::cout<<"Activate tha application "<<std::endl;

        integrationTestResult = app.Activate();
        app.cond.wait_for(lck, std::chrono::milliseconds(10000), [&] {
            return app.IsCorrectStatus(ara::ucm::pkgmgr::PackageManagerStatusType::kActivated);
        });

        if (!integrationTestResult)
        {
            std::cout<<"Rollback application"<<std::endl;
             
            integrationTestResult = app.Rollback();
            app.cond.wait_for(lck, std::chrono::milliseconds(10000), [&] {
                return app.IsCorrectStatus(ara::ucm::pkgmgr::PackageManagerStatusType::kRolledBack);
            });
        }
       
        std::cout<<"finish installing"<<std::endl;

        integrationTestResult = app.Finish();
        app.cond.wait_for(lck, std::chrono::milliseconds(10000), [&] {
            return app.IsCorrectStatus(ara::ucm::pkgmgr::PackageManagerStatusType::kIdle);
        });
    }

 

    // calling GetSwClusterInfo
    std::cout << "Calling GetSWClusterInfo "<< service->GetSwClusterInfo().get().SwInfo.size()<< std::endl;

    int index = 0;
    for (auto info : service->GetSwClusterInfo().get().SwInfo)
    {
        logger.LogWarn() << "Element[" << index << "]"
                         << "SwInfoName: " << info.Name;

        std::cout << info.Name << std::endl;

        std::cout << info.Version << std::endl;

        logger.LogWarn() << "Element[" << index << "]"
                         << "Version: " << info.Version;
        logger.LogWarn() << "Element[" << index << "]"
                         << "SwClusterStateType: " << [=](ara::ucm::pkgmgr::SwClusterStateType state) {
                                switch (state)
                                {
                                    using namespace std::string_literals;
                                case ara::ucm::pkgmgr::SwClusterStateType::kPresent:
                                    return "kPresent"s;
                                case ara::ucm::pkgmgr::SwClusterStateType::kAdded:
                                    return "kAdded"s;
                                case ara::ucm::pkgmgr::SwClusterStateType::kUpdated:
                                    return "kUpdated"s;
                                case ara::ucm::pkgmgr::SwClusterStateType::kRemoved:
                                    return "kRemoved"s;
                                default:
                                    return "Unknown"s;
                                }
                            }(info.State);
        index++;
    }
    if (index == 0)
    {
        logger.LogWarn() << "No SwClusterInfo";
    }

    ReportIntegrationTestResult(integrationTestResult);
}

int main(int, char **)
{
    std::cout << std::endl
              << std::endl;
    std::cout << "UCM SAMPLES DEMO STARTS" << std::endl;
    std::cout << "===========================" << std::endl;

    // report application state
    ara::exec::ExecutionClient exec_client;
    exec_client.ReportExecutionState(ara::exec::ExecutionState::kRunning);
    InitLogging(
        "UCMS", "UCM SAMPLE", ara::log::LogLevel::kVerbose, ara::log::LogMode::kConsole | ara::log::LogMode::kRemote);

    auto &logger = ara::log::CreateLogger("MAIN", "MAIN context", ara::log::LogLevel::kVerbose);
    logger.LogInfo() << "Starting PackageManager Sample app.";

    PackageManagerTestSequence();

    logger.LogInfo() << "Terminating PackageManager Sample app.";
    exec_client.ReportExecutionState(ara::exec::ExecutionState::kTerminating);
    return 0;
}
