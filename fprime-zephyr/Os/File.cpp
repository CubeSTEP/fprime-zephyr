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
        int descriptor;
        
        descriptor = fs_open(file, filepath, mode_flags);
        if (descriptor < 0) {
            DEBUG_PRINT("Error opening file %s: %d\n", filepath, descriptor);

            k_free(file);
            status = NOT_OPENED; // Set file status to NOT_OPENED for now
        }

        this->m_handle.m_file_descriptor = descriptor;

        return status;
    }

    void ZephyrFile::close() {}

    ZephyrFile::Status ZephyrFile::size(FwSizeType &size_result) {
        Status status = Status::NOT_SUPPORTED;
        return status;
    }

    ZephyrFile::Status ZephyrFile::position(FwSizeType &position_result) {
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

} // namespace File
} // namespace Stub
} // namespace Os
