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

#include "filesystem.h"

#include <dirent.h>
#include "boost/filesystem.hpp"

using error_code = boost::system::error_code;

namespace ara
{
namespace ucm
{
namespace pkgmgr
{

const ara::core::String Filesystem::PATH_SEPARATOR("/");

ara::core::Vector<ara::core::String> Filesystem::GetSubdirectories(const ara::core::String& dir) noexcept
{
    return Find(dir, DT_DIR);
}

ara::core::Vector<ara::core::String> Filesystem::GetFiles(const ara::core::String& dir) noexcept
{
    return Find(dir, DT_REG);
}

ara::core::Vector<ara::core::String> Filesystem::GetFilesRecursive(const ara::core::String& dir) noexcept
{
    ara::core::Vector<ara::core::String> entries;
    error_code ec;

    if (!boost::filesystem::is_directory(dir.c_str(), ec) || ec)
        return entries;

    auto iter = boost::filesystem::recursive_directory_iterator(dir.c_str(), ec);

    if (ec)
        return entries;

    while (iter != boost::filesystem::recursive_directory_iterator()) {
        if (boost::filesystem::is_regular_file(iter->status())) {
            entries.push_back(iter->path().native());
        }

        iter.increment(ec);

        if (ec)
            return entries;
    }

    return entries;
}

ara::core::Vector<ara::core::String> Filesystem::GetFilesStartingWith(const ara::core::String& dir,
    const ara::core::String& starting) noexcept
{
    ara::core::Vector<ara::core::String> files = GetFiles(dir);
    ara::core::Vector<ara::core::String> retFiles;
    for (auto file : files) {
        if (!ExtractFileName(file).compare(0, starting.size(), starting)) {
            retFiles.push_back(file);
        }
    }
    return retFiles;
}

ara::core::String Filesystem::BuildPath(const ara::core::String& base, const ara::core::String& additional) const
    noexcept
{
    ara::core::String subdirectory_name(base);
    subdirectory_name += PATH_SEPARATOR;
    subdirectory_name += additional;
    return subdirectory_name;
}

ara::core::String Filesystem::ExtractFileName(const ara::core::String& path) const noexcept
{
    ara::core::String application_name = trim(path, PATH_SEPARATOR);
    size_t last_path_separator_position(application_name.find_last_of(PATH_SEPARATOR));
    if (last_path_separator_position == ara::core::String::npos) {
        return application_name;
    } else {
        return application_name.substr(last_path_separator_position + 1, ara::core::String::npos);
    }
}

ara::core::String Filesystem::GetFilePrefix(const ara::core::String& path) const noexcept
{
    size_t last_path_separator_position = path.find_last_of(PATH_SEPARATOR);
    if (last_path_separator_position == ara::core::String::npos) {
        return path;
    } else {
        return path.substr(0, last_path_separator_position);
    }
}

Result<void> Filesystem::RemoveDirectory(const ara::core::String& targetDirectory) const noexcept
{
    if (!DoesDirectoryExist(targetDirectory)) {
        return Result<void>::FromError({FilesystemErrc::kNoSuchFileOrDirectory});
    }

    // log_.LogInfo() << "Removing directory " << targetDirectory;

    error_code ec;
    auto numDeletedItems = boost::filesystem::remove_all(targetDirectory.c_str(), ec);

    if (ec || 0 == numDeletedItems) {
        return Result<void>::FromError({FilesystemErrc::kRemoveDirectory, ec.value()});
    }

    return {};
}

bool Filesystem::DoesDirectoryExist(const ara::core::String& directoryPath) const noexcept
{
    error_code ec;
    return boost::filesystem::is_directory(directoryPath.c_str(), ec) && !ec;
}

Result<uint64_t> Filesystem::GetFreeDiskSpace(const ara::core::String& path) const noexcept
{
    error_code ec;
    boost::filesystem::space_info info = boost::filesystem::space(path.c_str(), ec);

    if (ec) {
        return Result<uint64_t>::FromError({FilesystemErrc::kGetFreeDiskSpace, ec.value()});
    }

    return static_cast<uint64_t>(info.available);
}

Result<void> Filesystem::MoveDirectory(const ara::core::String& sourceDirectory,
    const ara::core::String& targetDirectory) noexcept
{
    error_code ec;

    if (!DoesDirectoryExist(sourceDirectory) || DoesDirectoryExist(targetDirectory)) {
        return Result<void>::FromError({FilesystemErrc::kNoSuchFileOrDirectory});
    }

    if (!boost::filesystem::create_directory(targetDirectory.c_str(), ec) || ec) {
        return Result<void>::FromError({FilesystemErrc::kCreateDirectory, ec.value()});
    }

    ara::core::Vector<boost::filesystem::path> pathsToRename;

    ec.clear();
    auto dirIterator = boost::filesystem::recursive_directory_iterator(sourceDirectory.c_str(), ec);

    if (ec) {
        return Result<void>::FromError({FilesystemErrc::kCreateDirectoryIterator, ec.value()});
    }

    for (const auto& entry : dirIterator) {
        pathsToRename.push_back(entry.path());
    }

    for (auto& entryPath : pathsToRename) {
        ec.clear();
        auto entryPathStr = entryPath.string();
        auto filename = lexically_relative(entryPath, sourceDirectory.c_str());
        auto targetpath = BuildPath(targetDirectory, filename.string());

        // boost rename operation is already recursive. Therefore we need to check if a file has
        // already been moved
        if (DoesDirectoryExist(entryPathStr) || DoesFileExist(entryPathStr)) {
            // log_.LogInfo() << "Moving " << entryPath.string() << " to " << targetpath;
            boost::filesystem::rename(entryPath, targetpath.c_str(), ec);

            if (ec) {
                return Result<void>::FromError({FilesystemErrc::kRenameFile, ec.value()});
            }
        }
    }
    return {};
}

Result<uint64_t> Filesystem::GetDirectorySize(const ara::core::String& path) const noexcept
{
    if (!DoesDirectoryExist(path)) {
        return Result<uint64_t>::FromError({FilesystemErrc::kNoSuchFileOrDirectory});
    }

    error_code ec;
    uint64_t directory_size = 0;

    auto iterator = boost::filesystem::recursive_directory_iterator(path.c_str(), ec);
    if (ec) {
        return Result<uint64_t>::FromError({FilesystemErrc::kCreateDirectoryIterator, ec.value()});
    }

    for (; iterator != boost::filesystem::recursive_directory_iterator(); ++iterator) {

        ec.clear();
        bool isRegularFile = boost::filesystem::is_regular_file(*iterator, ec);

        if (ec) {
            return Result<uint64_t>::FromError({FilesystemErrc::kDetectRegularFile, ec.value()});
        } else if (isRegularFile) {
            directory_size += boost::filesystem::file_size(*iterator);
        }
    }

    return directory_size;
}

Result<void> Filesystem::CopyDirectory(const ara::core::String& sourceDirectory,
    const ara::core::String& targetDirectory) const noexcept
{
    if (!DoesDirectoryExist(sourceDirectory) || DoesDirectoryExist(targetDirectory)) {
        return Result<void>::FromError({FilesystemErrc::kNoSuchFileOrDirectory});
    }

    error_code ec;

    if (!boost::filesystem::create_directory(targetDirectory.c_str(), ec) || ec) {
        return Result<void>::FromError({FilesystemErrc::kCreateDirectory, ec.value()});
    }

    ec.clear();
    auto dirIterator = boost::filesystem::recursive_directory_iterator(sourceDirectory.c_str(), ec);

    if (ec) {
        return Result<void>::FromError({FilesystemErrc::kCreateDirectoryIterator, ec.value()});
    }

    for (const auto& entry : dirIterator) {
        ec.clear();

        auto& entryPath = entry.path();
        auto filename = lexically_relative(entryPath, sourceDirectory.c_str());
        auto targetpath = BuildPath(targetDirectory, filename.string());

        // log_.LogInfo() << "Copying " << entryPath.string() << " to " << targetpath;
        boost::filesystem::copy(entryPath, targetpath.c_str(), ec);

        if (ec) {
            return Result<void>::FromError({FilesystemErrc::kCopyDirectory, ec.value()});
        }
    }
    return {};
}

Result<void> Filesystem::CopyFile(const ara::core::String& sourceFile, const ara::core::String& targetFile) noexcept
{
    if (!DoesFileExist(sourceFile)) {
        return Result<void>::FromError({FilesystemErrc::kNoSuchFileOrDirectory});
    }

    if (DoesFileExist(targetFile)) {
        return Result<void>::FromError({FilesystemErrc::kFileAlreadyExists});
    }

    error_code ec;

    if (!boost::filesystem::is_regular_file(sourceFile.c_str(), ec) || ec) {
        return Result<void>::FromError({FilesystemErrc::kDetectRegularFile, ec.value()});
    }

    // log_.LogInfo() << "Copying " << sourceFile << " to " << targetFile;
    ec.clear();
    boost::filesystem::copy_file(sourceFile.c_str(), targetFile.c_str(), ec);

    if (ec) {
        return Result<void>::FromError({FilesystemErrc::kCopyFile, ec.value()});
    }

    return {};
}

bool Filesystem::DoesFileExist(const ara::core::String& filePath) noexcept
{
    error_code ec;
    return boost::filesystem::is_regular_file(filePath.c_str(), ec) && !ec;
}

ara::core::Vector<ara::core::String> Filesystem::Find(const ara::core::String& dir, unsigned char type) const noexcept
{
    ara::core::Vector<ara::core::String> entries;
    error_code ec;

    if (!boost::filesystem::is_directory(dir.c_str(), ec) || ec)
        return entries;

    auto iter = boost::filesystem::directory_iterator(dir.c_str(), ec);

    if (ec)
        return entries;

    while (iter != boost::filesystem::directory_iterator()) {
        if ((type == DT_DIR && boost::filesystem::is_directory(iter->status()))
            || (type == DT_REG && boost::filesystem::is_regular_file(iter->status()))) {
            entries.push_back(iter->path().native());
        }

        iter.increment(ec);

        if (ec)
            return entries;
    }

    return entries;
}

bool Filesystem::isCurrentOrParentDir(const ara::core::String& dirname) const noexcept
{
    return dirname == "." || dirname == "..";
}

boost::filesystem::path Filesystem::lexically_relative(const boost::filesystem::path& target,
    const boost::filesystem::path& base) const noexcept
{
    using boost::filesystem::path;
    namespace detail = boost::filesystem::detail;

    std::pair<path::iterator, path::iterator> mm = mismatch(target.begin(), target.end(), base.begin(), base.end());
    if (mm.first == target.begin() && mm.second == base.begin())
        return path();
    if (mm.first == target.end() && mm.second == base.end())
        return detail::dot_path();
    path tmp;
    for (; mm.second != base.end(); ++mm.second)
        tmp /= detail::dot_dot_path();
    for (; mm.first != target.end(); ++mm.first)
        tmp /= *mm.first;
    return tmp;
}

ara::core::String Filesystem::trim(const ara::core::String& text, const ara::core::String& chars_to_trim) const noexcept
{
    size_t first_char(text.find_first_not_of(chars_to_trim));
    size_t last_char(text.find_last_not_of(chars_to_trim));
    if (first_char == 0 && last_char == text.length() - 1) {
        return text;
    } else if ((first_char == ara::core::String::npos && last_char == ara::core::String::npos)
        || (last_char < first_char)) {
        // This string is either empty or contains nothing but trimmable characters
        return ara::core::String();
    } else {
        return text.substr(first_char, last_char - first_char + 1);
    }
}

std::pair<boost::filesystem::path::iterator, boost::filesystem::path::iterator> Filesystem::mismatch(
    boost::filesystem::path::iterator it1,
    boost::filesystem::path::iterator it1end,
    boost::filesystem::path::iterator it2,
    boost::filesystem::path::iterator it2end) const noexcept
{
    for (; it1 != it1end && it2 != it2end && *it1 == *it2;) {
        ++it1;
        ++it2;
    }
    return std::make_pair(it1, it2);
}

}  //  namespace pkgmgr
}  //  namespace ucm
}  //  namespace ara
