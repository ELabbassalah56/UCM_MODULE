#include "parsing/ucm_info.h"

<<<<<<< HEAD

=======
>>>>>>> bf25fabc8941baef3e8d8669df567de6c3a608ef
namespace ara
{
    namespace ucm
    {
        namespace pkgmgr
        {
            ucminfo::ucminfo()
            {
                ucminfo::collectUcmData();
            }

            ucminfo::~ucminfo()
            {
            }

            void ucminfo::collectUcmData()
            {
                
                mSummary = mGetUcmInfo.ucmInfo().at(0).replace(0, 1, "");
                mName = mGetUcmInfo.ucmInfo().at(1).replace(0, 1, "");
                mVersion = mGetUcmInfo.ucmInfo().at(2).replace(0, 1, "");
                mRelease = mGetUcmInfo.ucmInfo().at(3).replace(0, 1, "");
                mLicense = mGetUcmInfo.ucmInfo().at(4).replace(0, 1, "");
                mGroup = mGetUcmInfo.ucmInfo().at(5).replace(0, 1, "");
            }

<<<<<<< HEAD
            ara::core::String ucminfo::GetSummaryUcmInfo()
            {
                return mSummary;
            }
            ara::core::String ucminfo::GetNameUcmInfo()
            {
                return mName;
            }
            ara::core::String ucminfo::GetVersionUcmInfo()
            {
                return mVersion;
            }
            ara::core::String ucminfo::GetReleaseUcmInfo()
            {
                return mRelease;
            }
            ara::core::String ucminfo::GetLicenseUcmInfo()
            {
                return mLicense;
            }
            ara::core::String ucminfo::GetGroupUcmInfo()
=======
            string ucminfo::GetSummaryUcmInfo()
            {
                return mSummary;
            }
            string ucminfo::GetNameUcmInfo()
            {
                return mName;
            }
            string ucminfo::GetVersionUcmInfo()
            {
                return mVersion;
            }
            string ucminfo::GetReleaseUcmInfo()
            {
                return mRelease;
            }
            string ucminfo::GetLicenseUcmInfo()
            {
                return mLicense;
            }
            string ucminfo::GetGroupUcmInfo()
>>>>>>> bf25fabc8941baef3e8d8669df567de6c3a608ef
            {
                return mGroup;
            }
        } // namespace pkgmgr
    }     // namespace ucm
} // namespace ara