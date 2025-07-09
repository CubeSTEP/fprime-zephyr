// ======================================================================
// \title fprime-zephyr/Os/File.cpp
// \brief zephyr implementation for Os::File
// ======================================================================
// #include <Fw/Types/Assert.hpp>
// #include <unistd.h>
#include "fprime-zephyr/Os/File.hpp"
#include <Fw/Types/Assert.hpp>

#include <zephyr/kernel.h>
#include <zephyr/fs/fs.h>


#include <limits>           // for std::numeric_limits
#include <type_traits>      // for std::make_unsigned
// #include <Os/Posix/error.hpp> // TODO: Should either implement error.hpp or include Posix error.hpp

// Remove later
#include <cstdlib>
#define DEBUG_PRINT(x,...) printk(x,##__VA_ARGS__);

namespace Os {
namespace Zephyr {
namespace File {

    using UnsignedOffT = std::make_unsigned<off_t>::type;
    static const UnsignedOffT OFF_T_MAX_LIMIT = static_cast<UnsignedOffT>(std::numeric_limits<off_t>::max());
    using UnsignedSSizeT = std::make_unsigned<ssize_t>::type;
    static const UnsignedSSizeT SSIZE_T_MAX_LIMIT = static_cast<UnsignedSSizeT>(std::numeric_limits<ssize_t>::max());

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
                    status = FILE_EXISTS;
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

            // status = Os::Posix::errno_to_file_status(-file_status);
            status = NOT_OPENED;
            return status; // Set file status to NOT_OPENED for now.
        }

        this->m_handle.m_file_ptr = file;

        return status;
    }

    void ZephyrFile::close() {
        // if(ZephyrFileHandle::INVALID_FILE_DESCRIPTOR == this->m_handle.m_file_descriptor) {
        if(!this->_isOpen()) return;

        int close_res = fs_close(this->m_handle.m_file_ptr);
        if(close_res < 0) {
            DEBUG_PRINT("Error closing file: %d\n", close_res); // Remove Later
        }
        FW_ASSERT(close_res == 0, close_res);
        k_free(this->m_handle.m_file_ptr);
        this->m_handle.m_file_ptr = nullptr;
    }

    ZephyrFile::Status ZephyrFile::size(FwSizeType &size_result) {
        if (!this->_isOpen()) return Os::File::Status::NOT_OPENED;

        size_result = 0;
        FwSizeType current_position = 0;
        Status status = this->position(current_position);

        if (status != OP_OK) return status;

        FW_ASSERT(current_position <= OFF_T_MAX_LIMIT);

        // Seek to end to determine file size
        int seek_res = fs_seek(this->m_handle.m_file_ptr, 0, FS_SEEK_END);
        if (seek_res < 0) {
            return Os::File::Status::NOT_SUPPORTED; // TODO: Return correct status
        }

        FwSizeType end_of_file = 0;
        status = this->position(end_of_file);
        if (status != OP_OK) {
            // Try to restore pointer before returning, even on error
            fs_seek(this->m_handle.m_file_ptr, static_cast<off_t>(current_position), FS_SEEK_SET);
            return status;
        }

        // Restore file pointer to original position
        fs_seek(this->m_handle.m_file_ptr, static_cast<off_t>(current_position), FS_SEEK_SET);

        size_result = static_cast<FwSizeType>(end_of_file);
        return status;
    }

    ZephyrFile::Status ZephyrFile::position(FwSizeType &position_result) {
        if(!this->_isOpen()) return NOT_OPENED;

        Status status = OP_OK;
        position_result = 0;
        off_t tell_res = fs_tell(this->m_handle.m_file_ptr);
        if (tell_res < 0) { // TODO
            // status = Os::Posix::errno_to_file_status(-res);
            switch (tell_res) {
                case -EBADF:
                    status = NOT_OPENED; 
                    break;
                case -ENOTSUP:
                    status = NOT_SUPPORTED;
                    break;
                default:
                    status = NOT_SUPPORTED;
            }
        }
        // Protected by static assertion (FwSizeType >= off_t)
        position_result = static_cast<FwSizeType>(tell_res);
        return status;
    }

    ZephyrFile::Status ZephyrFile::preallocate(FwSizeType offset, FwSizeType length) {
        Status status = Status::NOT_SUPPORTED;
        return status;
    }

    ZephyrFile::Status ZephyrFile::seek(FwSignedSizeType offset, SeekType seekType) {
        if(!this->_isOpen()) return NOT_OPENED;

        Status status = OP_OK;
        if (offset > std::numeric_limits<off_t>::max()) {
            status = BAD_SIZE;
        } else {
            int seek_res = fs_seek(this->m_handle.m_file_ptr, static_cast<off_t>(offset), (seekType == SeekType::ABSOLUTE) ? FS_SEEK_SET : FS_SEEK_CUR);
            // int errno_store = errno;
            if (seek_res < 0) {
                status = NOT_SUPPORTED; // TODO
                // status = Os::Posix::errno_to_file_status(errno_store);
            } 
            // else if ((seekType == SeekType::ABSOLUTE) && (actual != offset)) {
            //     status = Os::File::Status::OTHER_ERROR;
            // }
        }
        return status;
    }

    ZephyrFile::Status ZephyrFile::flush() {
        if(!this->_isOpen()) return NOT_OPENED;

        Status status = OP_OK;

        int sync_res = fs_sync(this->m_handle.m_file_ptr);
        if (sync_res < 0) {
            status = NOT_SUPPORTED; // TODO
            // int errno_store = errno;
            // status = Os::Posix::errno_to_file_status(errno_store);
        }
        return status;
    }

    ZephyrFile::Status ZephyrFile::read(U8 *buffer, FwSizeType &size, ZephyrFile::WaitType wait) {
        if(!this->_isOpen()) return NOT_OPENED;
        
        Status status = OP_OK;
        FwSizeType accumulated = 0;
        // Loop up to 2 times for each by, bounded to prevent overflow
        const FwSizeType maximum = (size > (std::numeric_limits<FwSizeType>::max() / 2))
                                            ? std::numeric_limits<FwSizeType>::max()
                                            : size * 2;
        // POSIX APIs are implementation dependent when dealing with sizes larger than the signed return value
        // thus we ensure a clear decision: BAD_SIZE
        if (size > SSIZE_T_MAX_LIMIT) {
            return BAD_SIZE;
        }

        for (FwSizeType i = 0; i < maximum && accumulated < size; i++) {
            // char* for some posix implementations
            ssize_t read_size = fs_read(this->m_handle.m_file_ptr, reinterpret_cast<CHAR*>(&buffer[accumulated]), static_cast<size_t>(size - accumulated));
            // Non-interrupt error
            if (read_size < 0) {
                // int errno_store = errno;
                int errno_store = -read_size;
                // Interrupted w/o read, try again
                if (errno_store == EINTR) {
                    continue;
                }
                // status = Os::Posix::errno_to_file_status(errno_store);
                status = NOT_SUPPORTED; // TODO
                break;
            }
            // End-of-file
            else if (read_size == 0) {
                break;
            }
            accumulated += static_cast<FwSizeType>(read_size);
            // Stop looping when we had a good read and are not waiting
            if (!wait) {
                break;
            }
        }
        size = accumulated;
        return status;
    }

    ZephyrFile::Status ZephyrFile::write(const U8 *buffer, FwSizeType &size, ZephyrFile::WaitType wait) {
        if(!this->_isOpen()) return NOT_OPENED;

        Status status = OP_OK;
        FwSizeType accumulated = 0;
        // Loop up to 2 times for each by, bounded to prevent overflow
        const FwSizeType maximum = (size > (std::numeric_limits<FwSizeType>::max() / 2))
                                            ? std::numeric_limits<FwSizeType>::max()
                                            : size * 2;
        // POSIX APIs are implementation dependent when dealing with sizes larger than the signed return value
        // thus we ensure a clear decision: BAD_SIZE
        if (size > SSIZE_T_MAX_LIMIT) {
            return BAD_SIZE;
        }

        for (FwSizeType i = 0; i < maximum && accumulated < size; i++) {
            // char* for some posix implementations
            ssize_t write_size = fs_write(this->m_handle.m_file_ptr, reinterpret_cast<const CHAR*>(&buffer[accumulated]), static_cast<size_t>(size - accumulated));
            // Non-interrupt error
            if (write_size < 0) {
                int errno_store = -write_size;
                // Interrupted w/o write, try again
                if (errno_store == EINTR) {
                    continue;
                }
                // status = Os::Posix::errno_to_file_status(errno_store);
                status = NOT_SUPPORTED;
                break;
            }
            accumulated += static_cast<FwSizeType>(write_size);
        }
        size = accumulated;
        // When waiting, sync to disk
        if (wait) {
            int fsync_res = fs_sync(this->m_handle.m_file_ptr);
            if (fsync_res < 0) {
                // int errno_store = errno;
                // status = Os::Posix::errno_to_file_status(errno_store);
                status = NOT_SUPPORTED;
            }
        }
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
