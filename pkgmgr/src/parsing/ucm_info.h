#ifndef ARA_UCM_PKGMGR_UCM_INFO_H
#define ARA_UCM_PKGMGR_UCM_INFO_H

#include "parsing/ucm_parsing_info.h"
<<<<<<< HEAD
<<<<<<< HEAD
#include "ara/core/string.h"
#include "ara/core/future.h"

#include <iostream>

=======

#include <iostream>
#include <string>
>>>>>>> bf25fabc8941baef3e8d8669df567de6c3a608ef
=======

#include <iostream>
#include <string>
>>>>>>> bf25fabc8941baef3e8d8669df567de6c3a608ef


using namespace std;


namespace ara
{
    namespace ucm
    {
        namespace pkgmgr
        {
            
            //remember update path to relative path

<<<<<<< HEAD
<<<<<<< HEAD
            static ara::core::String pathSpecFile = "/usr/share/info/ucm-package-manager.spec";
=======
            static string pathSpecFile = "/home/user/Desktop/GP_VALEO_AP/ara-project-R19-11/build/tmp/work/i586-poky-linux/ucm-package-manager/1.0-r1/ucm-package-manager.spec";
>>>>>>> bf25fabc8941baef3e8d8669df567de6c3a608ef
=======
            static string pathSpecFile = "/home/user/Desktop/GP_VALEO_AP/ara-project-R19-11/build/tmp/work/i586-poky-linux/ucm-package-manager/1.0-r1/ucm-package-manager.spec";
>>>>>>> bf25fabc8941baef3e8d8669df567de6c3a608ef

            class ucminfo
            {
            private:
<<<<<<< HEAD
<<<<<<< HEAD
                ara::core::String mSummary;
                ara::core::String mName;
                ara::core::String mVersion;
                ara::core::String mRelease;
                ara::core::String mLicense;
                ara::core::String mGroup;
=======
=======
>>>>>>> bf25fabc8941baef3e8d8669df567de6c3a608ef
                string mSummary;
                string mName;
                string mVersion;
                string mRelease;
                string mLicense;
                string mGroup;
<<<<<<< HEAD
>>>>>>> bf25fabc8941baef3e8d8669df567de6c3a608ef
=======
>>>>>>> bf25fabc8941baef3e8d8669df567de6c3a608ef
                ucmParseInfo mGetUcmInfo{pathSpecFile};
                void collectUcmData();

            public:
<<<<<<< HEAD
<<<<<<< HEAD
                ucminfo() ;
                ~ucminfo();
                ara::core::String GetSummaryUcmInfo();
                ara::core::String GetNameUcmInfo();
                ara::core::String GetVersionUcmInfo();
                ara::core::String GetReleaseUcmInfo();
                ara::core::String GetLicenseUcmInfo();
                ara::core::String GetGroupUcmInfo();
=======
=======
>>>>>>> bf25fabc8941baef3e8d8669df567de6c3a608ef
                ucminfo();
                ~ucminfo();
                string GetSummaryUcmInfo();
                string GetNameUcmInfo();
                string GetVersionUcmInfo();
                string GetReleaseUcmInfo();
                string GetLicenseUcmInfo();
                string GetGroupUcmInfo();
<<<<<<< HEAD
>>>>>>> bf25fabc8941baef3e8d8669df567de6c3a608ef
=======
>>>>>>> bf25fabc8941baef3e8d8669df567de6c3a608ef
            };

        } // namespace pkgmgr
    }     // namespace ucm
} // namespace ara
#endif