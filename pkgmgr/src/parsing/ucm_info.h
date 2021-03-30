#ifndef ARA_UCM_PKGMGR_UCM_INFO_H
#define ARA_UCM_PKGMGR_UCM_INFO_H

#include "parsing/ucm_parsing_info.h"

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

            static string pathSpecFile = "/home/user/Desktop/GP_VALEO_AP/ara-project-R19-11/build/tmp/work/i586-poky-linux/ucm-package-manager/1.0-r1/ucm-package-manager.spec";

            class ucminfo
            {
            private:
                string mSummary;
                string mName;
                string mVersion;
                string mRelease;
                string mLicense;
                string mGroup;
                ucmParseInfo mGetUcmInfo{pathSpecFile};
                void collectUcmData();

            public:
                ucminfo();
                ~ucminfo();
                string GetSummaryUcmInfo();
                string GetNameUcmInfo();
                string GetVersionUcmInfo();
                string GetReleaseUcmInfo();
                string GetLicenseUcmInfo();
                string GetGroupUcmInfo();
            };

        } // namespace pkgmgr
    }     // namespace ucm
} // namespace ara
#endif