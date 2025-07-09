// ======================================================================
// \title fprime-zephyr/Os/File.cpp
// \brief zephyr implementation for Os::File
// ======================================================================
// #include <Fw/Types/Assert.hpp>
#include "fprime-zephyr/Os/File.hpp"
#include <Fw/Types/Assert.hpp>

#include <zephyr/kernel.h>
#include <zephyr/fs/fs.h>

// Remove later
#include <cstdlib>
#define DEBUG_PRINT(x,...) printk(x,##__VA_ARGS__);

namespace Os {
namespace Zephyr {
namespace File {

    ZephyrFile::ZephyrFile(const ZephyrFile& other) {

    }

    // ZephyrFile::~ZephyrFile() {
    //     this->close();
    // }

    ZephyrFile::Status ZephyrFile::open(const char *filepath, ZephyrFile::Mode open_mode, OverwriteType overwrite) {
        FW_ASSERT(filepath != nullptr);

        struct fs_file_t *file = reinterpret_cast<struct fs_file_t *>(k_malloc(sizeof(struct fs_file_t)));
        if (file == nullptr) {
            FW_ASSERT(0);
            return NO_SPACE;
        }
        
        int mode_flags = 0;
        Status status = OP_OK;

        switch (open_mode) {
            case OPEN_READ:
                mode_flags = FS_O_READ;
                break;

            case OPEN_WRITE:
            case OPEN_SYNC_WRITE:
                mode_flags = FS_O_WRITE | FS_O_CREATE;
                break;

            case OPEN_CREATE:

                // Check if file exists and if it should not be overwritten
                struct fs_dirent entry;

                if(fs_stat(filepath, &entry) == 0 && ZephyrFile::OverwriteType::NO_OVERWRITE) {
                    k_free(file);
                    file = nullptr;
                    status = Os::File::Status::FILE_EXISTS;
                    return status;
                }
                mode_flags = FS_O_WRITE | FS_O_CREATE | FS_O_TRUNC;
                break;

            case OPEN_APPEND:
                mode_flags = FS_O_WRITE | FS_O_CREATE | FS_O_APPEND;
                break;

            default:
                FW_ASSERT(0, open_mode);
                break;
        }
        fs_file_t_init(file);
        int file_status = fs_open(file, filepath, mode_flags);
        if (file_status < 0) {
            DEBUG_PRINT("Error opening file %s: %d\n", filepath, file_status); // Remove later

            
            return NOT_OPENED; // Set file status to NOT_OPENED for now.
        }

        // this->m_handle.m_file_descriptor = file_status;
        this->m_handle.m_file_ptr = file;

        return status;
    }

    void ZephyrFile::close() {
        // if(ZephyrFileHandle::INVALID_FILE_DESCRIPTOR == this->m_handle.m_file_descriptor) {
        if(!this->_isOpen()) return;

        int res = fs_close(this->m_handle.m_file_ptr);
        if(res < 0) {
            DEBUG_PRINT("Error closing file: %d\n", res); // Remove Later
        }
        FW_ASSERT(res == 0, res);
        k_free(this->m_handle.m_file_ptr);
        this->m_handle.m_file_ptr = nullptr;
        // this->m_handle.m_file_descriptor = ZephyrFileHandle::INVALID_FILE_DESCRIPTOR;
    }

    ZephyrFile::Status ZephyrFile::size(FwSizeType &size_result) {
        Status status = Status::NOT_SUPPORTED;
        return status;
    }

    ZephyrFile::Status ZephyrFile::position(FwSizeType &position_result) {
        // Status status = OP_OK;
        // position_result = 0;
        // off_t actual = ::fs_seek(this->m_handle.m_file_descriptor, 0, FS_SEEK_CUR);
        // if (ZephyrFileHandle::ERROR_RETURN_VALUE == actual) {
        //     int errno_store = errno;
        //     // status = Os::Zephyr::errno_to_file_status(errno_store);
        // }
        // // Protected by static assertion (FwSizeType >= off_t)
        // position_result = static_cast<FwSizeType>(actual);
        // return status;
        Status status = Status::NOT_SUPPORTED;
        return status;
    }

    ZephyrFile::Status ZephyrFile::preallocate(FwSizeType offset, FwSizeType length) {
        Status status = Status::NOT_SUPPORTED;
        return status;
    }

    ZephyrFile::Status ZephyrFile::seek(FwSignedSizeType offset, SeekType seekType) {
        Status status = Status::NOT_SUPPORTED;
        return status;
    }

    ZephyrFile::Status ZephyrFile::flush() {
        Status status = Status::NOT_SUPPORTED;
        return status;
    }

    ZephyrFile::Status ZephyrFile::read(U8 *buffer, FwSizeType &size, ZephyrFile::WaitType wait) {
        Status status = Status::NOT_SUPPORTED;
        return status;
    }

    ZephyrFile::Status ZephyrFile::write(const U8 *buffer, FwSizeType &size, ZephyrFile::WaitType wait) {
        Status status = Status::NOT_SUPPORTED;
        return status;
    }

    FileHandle* ZephyrFile::getHandle() {
        return &this->m_handle;
    }

    bool ZephyrFile::_isOpen() const {
        return (this->m_handle.m_file_ptr != nullptr);
    }

    void ZephyrFile::_freeFile() {
        if (this->m_handle.m_file_ptr == nullptr) return;

        k_free(this->m_handle.m_file_ptr);
        this->m_handle.m_file_ptr = nullptr;
    }

} // namespace File
} // namespace Stub
} // namespace Os
