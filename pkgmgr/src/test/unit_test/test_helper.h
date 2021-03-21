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

#ifndef ARA_UCM_PKGMGR_TEST_HELPER_H_
#define ARA_UCM_PKGMGR_TEST_HELPER_H_

#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include <iostream>
#include <fstream>

#include "ara/core/vector.h"
#include "ara/core/string.h"

#include "boost/filesystem.hpp"
#include "boost/property_tree/ptree.hpp"

#include "filesystem.h"
#include "package_manager_impl/package_manager_impl.h"

#include "transfer/streamable_software_package_factory.h"
#include "transfer/software_package_data.h"

#include "package_manager_state/package_manager_state.h"
#include "manifest_samples.h"
#include "exceptions.h"

#include "extraction/software_package_extractor.h"

#include "storage/reversible_action.h"
#include "storage/install_action.h"
#include "storage/processes_list_manager.h"

#include "parsing/application_list_builder.h"
#include "parsing/application.h"
#include "parsing/software_cluster_manifest_parser.h"
#include "parsing/software_package_parser.h"
#include "parsing/software_package.h"
#include "parsing/software_package_manifest.h"
#include "parsing/software_cluster_manifest.h"
#include "parsing/software_cluster_list_builder.h"

#include "boost/property_tree/json_parser.hpp"
#include "boost/property_tree/ptree.hpp"

#include "test/unit_test/manifest_samples.h"
#include "ara/ucm/pkgmgr/impl_type_actiontype.h"

namespace fs = boost::filesystem;
using boost::property_tree::ptree;
using boost::property_tree::write_json;

namespace ara
{
namespace ucm
{
namespace pkgmgr
{

static const ara::core::String kTestDataPath = "/usr/share/apdtest/ucm";
static const ara::core::String kTestWorkingDir = "/usr/var/apdtest/ucm";
static const ara::core::String kTestOptPath = "/usr/var/apdtest/ucm/opt";
static const ara::core::String kTestInstallSwPackagePath = kTestDataPath + "/SWP_TEST1_INS.zip";
static const ara::core::String kTestUninstallSwPackagePath = kTestDataPath + "/SWP_TEST1_REM.zip";
static const ara::core::String kTestUpdateSwPackagePath = kTestDataPath + "/SWP_TEST1_UPD.zip";
static const uint64_t kTestOptPathSize = 21834;

static const ara::core::String kSwclManifestName = "SWCL_MANIFEST.json";
static const ara::core::String kSwclDir = kTestOptPath + "/updateDir/swcls";
static const ara::core::String kSwcl_A_Dir = kSwclDir + "/swcl_a/1_0_0_0";
static const ara::core::String kSwcl_A_App1Proc1Dir = kSwcl_A_Dir + "/app1/process1";
static const ara::core::String kSwcl_A_App1Proc2Dir = kSwcl_A_Dir + "/app1/process2";
static const ara::core::String kSwcl_A_App2Proc1Dir = kSwcl_A_Dir + "/app2/process1";
static const ara::core::String kSwcl_B_Dir = kSwclDir + "/swcl_b/1_0_0_2";
static const ara::core::String kSwcl_B_App1Proc1Dir = kSwcl_B_Dir + "/app1/process1";
static const ara::core::String kSwcl_C_Dir = kSwclDir + "/swcl_c/1_0_0_5";
static const ara::core::String kSwcl_C_App1Proc1Dir = kSwcl_C_Dir + "/app1/process1";

inline void CreateFile(String const& filename)
{
    std::fstream file;
    file.open(filename.c_str(), std::fstream::out | std::fstream::trunc);
}

inline void CreateSwclManifestFile(String const& filename)
{
    ptree sampleSwclManifest = testdata::CreateSoftwareClusterManifest("swclX", "1.0.0");

    std::ofstream file;
    file.open(filename.c_str(), std::fstream::out | std::fstream::trunc);
    if (file.is_open()) {
        write_json(file, sampleSwclManifest);
        file.close();
    }
}

inline void DeleteFile(String const& filename)
{
    boost::system::error_code error;
    fs::remove(filename.c_str(), error);
}

inline void CreateFolder(String const& folder)
{
    boost::system::error_code error;
    fs::create_directory(folder.c_str(), error);
}

inline void CreateFolderTree(ara::core::String const& folderTree)
{
    boost::system::error_code error;
    fs::create_directories(folderTree.c_str(), error);
}

inline void DeleteFolder(String const& folder)
{
    boost::system::error_code error;
    fs::remove_all(folder.c_str(), error);
}

inline void CreateSWCLsFolderStructure()
{

    CreateFolderTree(kSwcl_A_App1Proc1Dir);
    CreateFolderTree(kSwcl_A_App1Proc2Dir);
    CreateFolderTree(kSwcl_A_App2Proc1Dir);
    CreateSwclManifestFile(kSwcl_A_Dir + "/" + kSwclManifestName);

    CreateFolderTree(kSwcl_B_App1Proc1Dir);
    CreateSwclManifestFile(kSwcl_B_Dir + "/" + kSwclManifestName);

    CreateFolderTree(kSwcl_C_App1Proc1Dir);
    CreateSwclManifestFile(kSwcl_C_Dir + "/" + kSwclManifestName);
}

inline void RemoveSWCLsFolderStructure()
{
    DeleteFolder(kSwclDir + "/swcl_a");
    DeleteFolder(kSwclDir + "/swcl_b");
    DeleteFolder(kSwclDir + "/swcl_c");
}

inline boost::property_tree::ptree StringToPTree(const ara::core::String& data)
{
    boost::property_tree::ptree manifestTree;
    std::stringstream stream(data.c_str());
    boost::property_tree::read_json(stream, manifestTree);
    return manifestTree;
}

class SoftwarePackageExtractorMock : public SoftwarePackageExtractor
{
public:
    SoftwarePackageExtractorMock()
    {}

    MOCK_METHOD2(Extract, bool(const ara::core::String& archivePath, const ara::core::String& extractionDirectory));
    MOCK_METHOD0(Cleanup, void());
};

class FilesystemMock : public Filesystem
{
public:
    FilesystemMock()
    {}

    MOCK_METHOD1(GetFreeDiskSpace, uint64_t(const ara::core::String&));
    MOCK_METHOD2(CopyDirectory, bool(const ara::core::String&, const ara::core::String&));
    MOCK_METHOD1(RemoveDirectory, bool(const ara::core::String&));
    MOCK_METHOD1(GetDirectorySize, uint64_t(const ara::core::String&));
};

class ApplicationMock : public Application
{
public:
    ApplicationMock(const ara::core::String& prefix,
        const ara::core::String& executable,
        const ara::core::String& applicationName,
        const ara::core::String& manifestPath,
        const Version& version = Version(1, 0))
        : Application(prefix, executable, applicationName, manifestPath, version)
    {}

    void SetDependencies(const ara::core::Vector<ara::core::String>& depApps)
    {
        this->dependencies_ = depApps;
    }

protected:
    // we do not need the manifest content here
    void ParseApplicationManifest() override
    {}
};

class ReversibleActionStub : public ReversibleAction
{
public:
    explicit ReversibleActionStub(const ara::core::String& swclPath,
        const SoftwarePackage& package,
        Result<void, ErrorCode> result)
        : ReversibleAction(swclPath, package)
        , result_(result)
    {}

    Result<void, ErrorCode> Execute()
    {
        return result_;
    }

    Result<void, ErrorCode> RevertChanges()
    {
        return {};
    }

    Result<void, ErrorCode> CommitChanges()
    {
        return {};
    }

    Result<void, ErrorCode> result_;
};

class ApplicationListBuilderMock : public ApplicationListBuilder
{
public:
    ApplicationListBuilderMock()
        : ApplicationListBuilder(fs_, "")
    {
        ApplicationMock calc("", "", "Calculator", "");
        ApplicationMock dm("", "", "DiagnosticManager", "");
        apps.push_back(dm);
        apps.push_back(calc);
    }

    MOCK_CONST_METHOD0(GetApplications, const ara::core::Vector<Application>&());

    // Usage of GMOCK_METHOD0_ macro instead of MOCK_METHOD0 due to incompatibility with noexcept modifier
    GMOCK_METHOD0_(, noexcept, , RefreshList, void());

    ara::core::Vector<Application> apps;
    Filesystem fs_;
};

class SoftwareClusterMock : public SoftwareCluster
{
public:
    SoftwareClusterMock(const SoftwareClusterManifest swclManifest, const ara::core::String& manifestPath)
        : SoftwareCluster(swclManifest, manifestPath)
    {}
};

class SoftwareClusterListBuilderMock : public SoftwareClusterListBuilder
{
public:
    SoftwareClusterListBuilderMock()
        : SoftwareClusterListBuilder(fs_, "")
    {}

    void AddSwcl(const SoftwareCluster& swcl)
    {
        this->swcls_.push_back(swcl);
    }

    Filesystem fs_;
};

class SoftwarePackageParserStub : public SoftwarePackageParser
{
public:
    explicit SoftwarePackageParserStub(const SoftwarePackageManifest& swPackageManifest)
        : swPackageManifest_(swPackageManifest)
    {}

    std::unique_ptr<SoftwarePackage> Parse(const ara::core::String& extractionDirectory) override
    {
        if (invalidManifest_) {
            throw exception::InvalidManifestException("");
        }
        ara::core::String swclManifestPath = extractionDirectory + "/SWCL_MANIFEST.json";

        SoftwareCluster swCluster(swclManifest, swclManifestPath);

        return std::make_unique<SoftwarePackage>(swPackageManifest_, extractionDirectory, swCluster);
    }

    void SetInvalidManifestFlag(bool doesThrowInvalidManifestException)
    {
        invalidManifest_ = doesThrowInvalidManifestException;
    }

private:
    SoftwareClusterManifest swclManifest{"category",
        "changes",
        "license",
        Version("0.5.0"),
        "the_swcl_name",
        "uuid",
        "vendorID",
        Version("1.0.0"),
        {"dependentSoftwareClusterA", "dependentSoftwareClusterB", "dependentSoftwareClusterC"}};
    SoftwarePackageManifest swPackageManifest_;
    ara::core::Vector<Application> applications_;
    bool invalidManifest_{false};
};

class SoftwarePackageMock : public SoftwarePackage
{
public:
    explicit SoftwarePackageMock(SoftwarePackageManifest& manifest)
        : SoftwarePackage(manifest, "", softwareClster_)
    {
        ON_CALL(*this, GetManifest()).WillByDefault(testing::ReturnRef(manifest));
    }

    explicit SoftwarePackageMock(SoftwarePackageManifest& unused, ara::core::String path, SoftwareCluster swcluster)
        : SoftwarePackage(unused, path, swcluster)
    {
        ON_CALL(*this, GetManifest()).WillByDefault(testing::ReturnRef(unused));
    }

    MOCK_CONST_METHOD0(GetPath, const ara::core::String&());
    MOCK_CONST_METHOD0(GetManifest, const SoftwarePackageManifest&());
    MOCK_CONST_METHOD0(GetApplications, const ara::core::Vector<Application>&());
    MOCK_CONST_METHOD0(GetUncompressedSize, uint64_t());

private:
    SoftwareClusterManifest swclManifest_{"category",
        "changes",
        "license",
        Version("0.5.0"),
        "the_swcl_name",
        "uuid",
        "vendorID",
        Version("1.0.0"),
        {"dependentSoftwareClusterA", "dependentSoftwareClusterB", "dependentSoftwareClusterC"}};
    SoftwareCluster softwareClster_{swclManifest_, ""};
};

class ProcessesListManagerMock : public ProcessesListManager
{
public:
    explicit ProcessesListManagerMock()
        : ProcessesListManager(kTestOptPath)
    {}
    MOCK_METHOD0(UpdateProcessesList, void());
};

class SoftwarePackageDataMock : public SoftwarePackageData
{
public:
    SoftwarePackageDataMock()
        : SoftwarePackageData(1)
        , defaultID(1)
    {
        ON_CALL(*this, GetID()).WillByDefault(::testing::ReturnRef(defaultID));
    }

    MOCK_CONST_METHOD0(GetID, TransferIdType const&());
    // MOCK_CONST_METHOD0(GetState, SwPackageStateType const&());
    MOCK_METHOD1(TransferStart, TransferStartReturnType(const std::uint32_t& size));
    // MOCK_METHOD2(TransferData, TransferDataReturnType(const ByteVectorType&, const std::uint32_t& blockCounter));
    MOCK_METHOD0(TransferExit, TransferExitReturnType());
    // MOCK_CONST_METHOD0(GetPackagePath, ara::core::String());
    MOCK_METHOD0(GetStreamable, std::unique_ptr<Streamable>&());
    MOCK_CONST_METHOD0(GetStreamable, std::unique_ptr<Streamable> const&());

    TransferStartReturnType CallTransferStart(const std::uint32_t& size)
    {
        return SoftwarePackageData::TransferStart(size);
    }

    TransferIdType defaultID;
};

class StreamableSoftwarePackageFactoryStub : public StreamableSoftwarePackageFactory
{
public:
    StreamableSoftwarePackageFactoryStub()
        : swPackageMock(std::make_unique<::testing::NiceMock<SoftwarePackageDataMock>>())
    {}

    std::unique_ptr<StreamableSoftwarePackage> Create(TransferIdType) override
    {
        return std::move(swPackageMock);
    }

    std::unique_ptr<::testing::NiceMock<SoftwarePackageDataMock>> swPackageMock;
};

class TransferInstanceMock : public Streamable
{
public:
    TransferInstanceMock()
    {}

    MOCK_METHOD1(TransferStart, TransferStartReturnType(const std::uint32_t& size));
    MOCK_METHOD2(TransferData, TransferDataReturnType(const ByteVectorType& data, const std::uint32_t& blockCounter));
    MOCK_METHOD0(TransferExit, TransferExitReturnType());
    MOCK_CONST_METHOD0(GetPackagePath, ara::core::String());
    MOCK_METHOD0(DeleteTransfer, DeleteTransferReturnType());
};

class SWCLManagerStub : public ::testing::NiceMock<SWCLManager>
{
public:
    MOCK_METHOD0(IndexSWCLs, void());
    MOCK_CONST_METHOD0(GetPresentSWCLs, ara::core::Vector<SoftwareCluster>());
    MOCK_CONST_METHOD0(GetSWCLsForActivation, ara::core::Vector<SoftwareCluster>());
    MOCK_CONST_METHOD0(GetSWCLsChangeInfo, const SwClusterInfoVectorType&());
    MOCK_METHOD1(AddSWCLChangeInfo, void(SoftwarePackage const& softwarePackage));
    MOCK_METHOD0(ResetSWCLChangeInfo, void());
};

}  // namespace pkgmgr
}  // namespace ucm
}  // namespace ara

#endif  // ARA_UCM_PKGMGR_TEST_HELPER_H_
