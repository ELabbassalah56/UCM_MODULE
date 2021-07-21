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

#include "extraction/lib_poco_zip_extractor.h"

#include "Poco/Delegate.h"
#include "Poco/Zip/Decompress.h"
#include "Poco/Zip/ZipLocalFileHeader.h"
#include "ara/log/logging.h"
#include "ara/core/string.h"
#include <string>
#include <fstream>

namespace ara
{
namespace ucm
{
namespace pkgmgr
{

static ara::log::Logger& logger()
{
    static ara::log::Logger& loggerInstance = ara::log::CreateLogger("LPZE", "LibPocoZipExtractor context");
    return loggerInstance;
}

LibPocoZipExtractor::LibPocoZipExtractor(Filesystem& fs)
    : SoftwarePackageExtractor()
    , fs_(fs)
{}

void LibPocoZipExtractor::OnDecompressOk(const void* pSender, LibPocoZipExtractor::DecompressOkInfo& info)
{
    (void)pSender;
    isProcessed_ = true;
    logger().LogInfo() << "Successfully extracted: " << info.first.getFileName().c_str();
}

void LibPocoZipExtractor::OnDecompressError(const void* pSender, LibPocoZipExtractor::DecompressErrorInfo& info)
{
    (void)pSender;
    isSuccessful_ = false;
    logger().LogInfo() << "Failed to extract: " << info.second.c_str();
}

bool LibPocoZipExtractor::Extract(const ara::core::String& archivePath, const ara::core::String& extractionDirectory)
{
    try {
        lastExtractionDirectory_ = extractionDirectory;
        isSuccessful_ = true;
        isProcessed_ = false;

        // create input file based stream
        std::ifstream arc(archivePath.c_str(), std::ios::binary);
        // create a custom deleter for Decompress object smart pointer
        // (std::unique_ptr)
        auto deleter = [obj = this](Poco::Zip::Decompress * pDec) noexcept
        {
            try {
                pDec->EOk
                    -= Poco::Delegate<LibPocoZipExtractor, DecompressOkInfo>(obj, &LibPocoZipExtractor::OnDecompressOk);
                pDec->EError -= Poco::Delegate<LibPocoZipExtractor, DecompressErrorInfo>(
                    obj, &LibPocoZipExtractor::OnDecompressError);

            } catch (...) {
                logger().LogInfo() << "Failed to release a Poco Library resource";
            }
            delete pDec;
        };
        // create a Decompress object and assign it to raw pointer
        auto decPtr = new Poco::Zip::Decompress(arc, Poco::Path(extractionDirectory.c_str()));
        // create a smart pointer from raw pointer using custom deleter
        auto decompress = std::unique_ptr<Poco::Zip::Decompress, decltype(deleter)>(decPtr, deleter);
        // assign handlers
        decompress->EOk
            += Poco::Delegate<LibPocoZipExtractor, DecompressOkInfo>(this, &LibPocoZipExtractor::OnDecompressOk);
        decompress->EError
            += Poco::Delegate<LibPocoZipExtractor, DecompressErrorInfo>(this, &LibPocoZipExtractor::OnDecompressError);
        // actually decompress an archive
        decompress->decompressAllFiles();
        if (isProcessed_ && isSuccessful_) {
            return true;
        }
    } catch (const Poco::Exception& e) {
        logger().LogInfo() << "Failed to extract an archive: " << e.displayText();
    } catch (const std::exception& e) {
        logger().LogInfo() << "Failed to extract an archive: " << e.what();
    } catch (...) {
        logger().LogInfo() << "Failed to extract an archive " << archivePath;
    }
    Cleanup();
    return false;
}

void LibPocoZipExtractor::Cleanup()
{
    if (!lastExtractionDirectory_.empty() && fs_.DoesDirectoryExist(lastExtractionDirectory_)) {
        fs_.RemoveDirectory(lastExtractionDirectory_);
    }
}

}  // namespace pkgmgr
}  // namespace ucm
}  // namespace ara
