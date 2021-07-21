#ifndef ARA_UCM_PKGMGR_UCM_INFO_H
#define ARA_UCM_PKGMGR_UCM_INFO_H

#include "parsing/ucm_parsing_info.h"
#include "ara/core/string.h"
#include "ara/core/future.h"
#include <iostream>
#include <string>


using namespace std;


namespace ara
{
    namespace ucm
    {
        namespace pkgmgr
        {
            
            //remember update path to relative path

            static ara::core::String pathSpecFile = "/usr/share/apdtest/ucm/ucm-package-manager.spec";
            class ucminfo
            {
            private:
                ara::core::String mSummary;
                ara::core::String mName;
                ara::core::String mVersion;
                ara::core::String mRelease;
                ara::core::String mLicense;
                ara::core::String mGroup;
                ucmParseInfo mGetUcmInfo{pathSpecFile};
                void collectUcmData();

            public:

                ucminfo() ;
                ~ucminfo();
                ara::core::String GetSummaryUcmInfo();
                ara::core::String GetNameUcmInfo();
                ara::core::String GetVersionUcmInfo();
                ara::core::String GetReleaseUcmInfo();
                ara::core::String GetLicenseUcmInfo();
                ara::core::String GetGroupUcmInfo();
            };

        } // namespace pkgmgr
    }     // namespace ucm
} // namespace ara
#endif