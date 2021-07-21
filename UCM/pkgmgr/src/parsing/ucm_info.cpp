#include "parsing/ucm_info.h"

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
            {
                return mGroup;
            }
        } // namespace pkgmgr
    }     // namespace ucm
} // namespace ara