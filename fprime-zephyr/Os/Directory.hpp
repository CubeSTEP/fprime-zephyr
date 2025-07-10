// ======================================================================
// \title fprime-zephyr/Os/Directory.hpp
// \brief zephyr definitions for Os::Directory
// ======================================================================
#ifndef OS_ZEPHYR_DIRECTORY_HPP
#define OS_ZEPHYR_DIRECTORY_HPP

#include <Os/Directory.hpp>
#include <zephyr/fs/fs.h>

namespace Os {
namespace Zephyr {
namespace Directory {

struct ZephyrDirectoryHandle : public DirectoryHandle {
  struct fs_dir_t* m_dir_ptr = nullptr;
};

//! \brief zephyr implementation of Os::Directory
//!
//! Zephyr implementation of `DirectoryInterface` for use as a delegate class handling error-only file operations.
//!
class ZephyrDirectory : public DirectoryInterface {
  public:
    //! \brief constructor
    ZephyrDirectory() = default;

    //! \brief destructor
    ~ZephyrDirectory() override = default;

    //! \brief return the underlying Directory handle (implementation specific)
    //! \return internal Directory handle representation
    DirectoryHandle* getHandle() override;

    // ------------------------------------------------------------
    // Implementation-specific Directory member functions
    // ------------------------------------------------------------

    //! \brief Open or create a directory
    //!
    //! Using the path provided, this function will open or create a directory. 
    //! Use OpenMode::READ to open an existing directory and error if the directory is not found
    //! Use OpenMode::CREATE_IF_MISSING to open a directory, creating the directory if it doesn't exist
    //! Use OpenMode::CREATE_EXCLUSIVE to open a directory, creating the directory and erroring if it already exists
    //!
    //! It is invalid to pass `nullptr` as the path.
    //! It is invalid to supply `mode` as a non-enumerated value.
    //!
    //! \param path: path of directory to open
    //! \param mode: enum (READ, CREATE_IF_MISSING, CREATE_EXCLUSIVE). See notes above for more information
    //! \return status of the operation
    Status open(const char* path, OpenMode mode) override;

    //! \brief Check if Directory is open or not
    //! \return true if Directory is open, false otherwise
    bool isOpen();

    //! \brief Rewind directory stream
    //!
    //! Each read operation moves the seek position forward. This function resets the seek position to the beginning.
    //!
    //! \return status of the operation
    Status rewind() override;

    //! \brief Get next filename from directory stream
    //!
    //! Writes at most buffSize characters of the file name to fileNameBuffer.
    //! This function skips the current directory (.) and parent directory (..) entries.
    //! Returns NO_MORE_FILES if there are no more files to read from the buffer.
    //!
    //! It is invalid to pass `nullptr` as fileNameBuffer.
    //!
    //! \param fileNameBuffer: buffer to store filename
    //! \param buffSize: size of fileNameBuffer
    //! \return status of the operation
    Status read(char * fileNameBuffer, FwSizeType buffSize) override;


    //! \brief Close directory
    void close() override;


  private:
    //! Handle for ZephyrDirectory
    ZephyrDirectoryHandle m_handle;
};

} // namespace Directory
} // namespace Zephyr
} // namespace Os
#endif // OS_ZEPHYR_DIRECTORY_HPP