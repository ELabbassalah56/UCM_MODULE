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

#ifndef ARA_UCM_PKGMGR_FILESYSTEM_H_
#define ARA_UCM_PKGMGR_FILESYSTEM_H_

#include <cstdint>
#include "ara/log/logging.h"
#include "boost/filesystem.hpp"
#include "ara/core/result.h"
#include "ara/core/error_code.h"
#include "ara/core/error_domain.h"
#include "ara/core/exception.h"
#include "ara/core/string.h"
#include "ara/core/vector.h"

using ara::core::Result;
using ara::core::ErrorDomain;
using ara::core::ErrorCode;

namespace ara
{
namespace ucm
{
namespace pkgmgr
{

/// @uptrace{SWS_CORE_00501}
enum class FilesystemErrc : ErrorDomain::CodeType
{
    kRemoveDirectory = 1,
    kNoSuchFileOrDirectory,
    kGetFreeDiskSpace,
    kCreateDirectory,
    kCreateDirectoryIterator,
    kRenameFile,
    kDetectRegularFile,
    kCopyDirectory,
    kFileAlreadyExists,
    kCopyFile
};

#if defined(__GNUC__)
#    pragma GCC diagnostic push
#    pragma GCC diagnostic ignored "-Wnon-virtual-dtor"
#endif

/// @uptrace{SWS_CM_10354}
class FilesystemException : public ara::core::Exception
{
public:
    explicit FilesystemException(ErrorCode&& err)
        : ara::core::Exception(std::move(err))
    {}
};

inline std::ostream& operator<<(std::ostream& out, FilesystemException const& ex)
{
    return (out << "FilesystemException: " << ex.Error() << " (" << ex.what() << ")");
}

class FilesystemErrorDomain : public ErrorDomain
{
    constexpr static ErrorDomain::IdType kId = 0x029f881224e69999LL;

public:
    using Errc = FilesystemErrc;
    using Exception = FilesystemException;

    constexpr FilesystemErrorDomain() noexcept
        : ErrorDomain(kId)
    {}

    char const* Name() const noexcept override
    {
        return "Filesystem";
    }

    char const* Message(ErrorDomain::CodeType errorCode) const noexcept override
    {
        Errc const code = static_cast<Errc>(errorCode);

        switch (code) {
        case Errc::kRemoveDirectory:
            return "error removing directory";

        case Errc::kNoSuchFileOrDirectory:
            return "no such file or directory";

        case Errc::kGetFreeDiskSpace:
            return "error getting free disk space";

        case Errc::kCreateDirectory:
            return "error creating directory";

        case Errc::kCreateDirectoryIterator:
            return "error creating directory iterator";

        case Errc::kRenameFile:
            return "rename error";

        case Errc::kDetectRegularFile:
            return "regular file detection error";

        case Errc::kCopyDirectory:
            return "error copying directory";

        case Errc::kFileAlreadyExists:
            return "file already exists";

        case Errc::kCopyFile:
            return "error copying file";

        default:
            return "unknown filesystem error";
        }
    }

    void ThrowAsException(ErrorCode const& errorCode) const noexcept(false) override
    {
        ara::core::ThrowOrTerminate<Exception>(errorCode);
    }
};

#if defined(__GNUC__)
#    pragma GCC diagnostic pop
#endif

namespace internal
{
constexpr FilesystemErrorDomain g_filesystemErrorDomain;
}

inline constexpr ErrorDomain const& GetFilesystemDomain()
{
    return internal::g_filesystemErrorDomain;
}

inline constexpr ErrorCode MakeErrorCode(FilesystemErrorDomain::Errc code,
    ErrorDomain::SupportDataType data)
{
    return ErrorCode(static_cast<ErrorDomain::CodeType>(code), GetFilesystemDomain(), data);
}

/// @brief This class contains the additions to the copied filesystem code
/// that have been implemented by UCM
class Filesystem
{
public:
    static const ara::core::String PATH_SEPARATOR;

    /// @brief Get the list of sub-directories at the path specified as parameter
    ///
    /// @param dir Directory to search directories in
    ///
    /// @return A vector of strings containing the sub-directories
    ara::core::Vector<ara::core::String> GetSubdirectories(const ara::core::String& dir) noexcept;

    /// @brief Get the list of files at the path specified as parameter
    ///
    /// @param dir Directory to search files in
    ///
    /// @returns A vector of strings containing the file names of the files available at the given path
    ara::core::Vector<ara::core::String> GetFiles(const ara::core::String& dir) noexcept;

    /// @brief Get the recursive list of filepaths to all files in the directory
    ///
    /// @param dir Directory to search in
    ///
    /// @returns List of full filepaths
    ara::core::Vector<ara::core::String> GetFilesRecursive(const ara::core::String& dir) noexcept;

    /// @brief Get the list of files at the path specified as parameter starting with the given prefix
    ///
    /// @param dir Directory to search files in
    /// @param starting Name prefix
    ///
    /// @returns A vector of strings containing the file names of the files available at the given path and starting
    /// with
    /// the given prefix
    ara::core::Vector<ara::core::String> GetFilesStartingWith(const ara::core::String& dir,
        const ara::core::String& starting) noexcept;

    /// @brief Builds the path string by appending one part to another
    ///
    /// BuildPath("path/to/file", "additional/path") = "path/to/file/additional/path"
    ///
    /// @param base Path to extend
    /// @param additional path to append
    ///
    /// @returns A string containing the path
    ara::core::String BuildPath(const ara::core::String& base, const ara::core::String& additional) const noexcept;

    /// @brief Extract the file name from the given path
    ///
    /// @param path Path to file
    ///
    /// @returns A string containing the file name
    ara::core::String ExtractFileName(const ara::core::String& path) const noexcept;

    /// @brief Get the file prefix from the given path
    ///
    /// @param path Path to a file
    ///
    /// @returns A string containing the file prefix
    ara::core::String GetFilePrefix(const ara::core::String& path) const noexcept;

    /// @brief Removes a directory recursively
    ///
    /// @param targetDirectory Path to the folder to remove
    ///
    /// @return true if the directory was successfully removed,
    /// false otherwise (non existent or not removed)
    Result<void> RemoveDirectory(const ara::core::String& targetDirectory) const noexcept;

    /// @brief Returns the size in bytes of the given directory.
    ///
    /// @param path The absolute path to a directory.
    ///
    /// @return The size in bytes of the given directory or -1 if the directory does not exist
    Result<uint64_t> GetDirectorySize(const ara::core::String& path) const noexcept;

    /// @brief Returns the amount of free disk space available at the given path in number of bytes
    ///
    /// @param path The target path for calculation free disk space
    ///
    /// @return The number of free disk space in bytes available at the given path
    Result<uint64_t> GetFreeDiskSpace(const ara::core::String& path) const noexcept;

    /// brief Moves the entire directory including all contained files/directories
    /// located at sourceDirectory to targetDirectory
    /// @note If a directory already exists at the target location, the operation is aborted
    /// and no files will be overwritten
    ///
    /// @param sourceDirectory Path to the source folder
    /// @param targetDirectory Path to the target folder
    ///
    /// @return true if the directory has been moved, false otherwise
    Result<void> MoveDirectory(const ara::core::String& sourceDirectory,
        const ara::core::String& targetDirectory) noexcept;

    /// @brief Copies the directory at sourceDirectory recursively to targetDirectory
    /// @note If a directory already exists at the target location, the operation is aborted
    /// and no files will be overwritten
    ///
    /// @param sourceDirectory The path to the directory that should be copied
    /// @param targetDirectory The destination path to the new location
    ///
    /// @return true if the directory was successfully copied, else false
    Result<void> CopyDirectory(const ara::core::String& sourceDirectory, const ara::core::String& targetDirectory) const
        noexcept;

    /// @brief Checks if a file exists at the given path
    ///
    /// @param filePath The path to the file that is checked for existence
    ///
    /// @return true if a file exists at the given path, else false
    bool DoesFileExist(const ara::core::String& filePath) noexcept;

    /// @brief Checks if a directory exists at the given directory path
    ///
    /// @param directoryPath Full path to a directory
    ///
    /// @return true if a directory exists at the given path, else false
    bool DoesDirectoryExist(const ara::core::String& directoryPath) const noexcept;

    /// @brief Copies a file, identified by the sourcefile path parameter to the path
    /// specified by the targetfile path parameter
    /// @note If a file already exists at the target location, it will not be overwritten,
    /// the copy operation is aborted.
    ///
    /// @param sourceFile The path to the file to be copied
    /// @param targetFile The destination path
    ///
    /// @return true if the file has been copied successfully, else false
    Result<void> CopyFile(const ara::core::String& sourceFile, const ara::core::String& targetFile) noexcept;

protected:
    // TBD: Logging in this class crashes and therefore is commented out. Figure out why it crashes
    // ara::log::Logger& log_{ara::log::CreateLogger("FLSY", "Filesystem")};

    /// @brief Helper function to find the files/sub-directories at the given path
    /// @note This call wrappes the opendir and readdir system calls
    ///
    /// @param dir Directory where to find
    /// @param type Identifier of object type to find
    ///
    /// @return A vector of strings containing the file/sub-directory names
    ara::core::Vector<ara::core::String> Find(const ara::core::String& dir, unsigned char type) const noexcept;

    /// @brief Checks if dirname current or parent directory
    ///
    /// @param dirname Directory to check
    ///
    /// @return True if any of those directory, false otherwise
    bool isCurrentOrParentDir(const ara::core::String& dirname) const noexcept;

    /// @brief build relative path from base to target
    /// lexically_relative(path("/a/d"), path("/a/b/c")) == "../../d"
    /// lexically_relative(path("a/b/c"), path("a/b/c/x/y") == "../.."
    /// @note copy-paste from boost::filesystem for adding noexcept
    ///
    /// @param target Destination path
    /// @param base Start path
    ///
    /// @return path to move from base to target
    boost::filesystem::path lexically_relative(const boost::filesystem::path& target,
        const boost::filesystem::path& base) const noexcept;

private:
    /// @brief Trims all characters occuring in chars_to_trim from the left and right end of text.
    /// More formally: At either end of text the longest substring consisting only of characters listed in chars_to_trim
    /// is removed.
    ///
    /// @param text The string to be trimmed
    /// @param chars_to_trim Characters to remove.
    ///
    /// @return trimmed string
    ara::core::String trim(const ara::core::String& text, const ara::core::String& chars_to_trim) const noexcept;

    /// @brief find first position of different elements in two sequences
    /// @note helper function for lexically_relative
    ///
    /// @param it1 start iterator to first sequence
    /// @param it1end end iterator to first sequence
    /// @param it2 start iterator to second sequence
    /// @param it2end end iterator to second sequence
    ///
    /// @return pair of iterators to first distinguishing elements or end iterators in case of equality
    std::pair<boost::filesystem::path::iterator, boost::filesystem::path::iterator> mismatch(
        boost::filesystem::path::iterator it1,
        boost::filesystem::path::iterator it1end,
        boost::filesystem::path::iterator it2,
        boost::filesystem::path::iterator it2end) const noexcept;
};

}  // namespace pkgmgr
}  // namespace ucm
}  // namespace ara

#endif  // ARA_UCM_PKGMGR_FILESYSTEM_H_
