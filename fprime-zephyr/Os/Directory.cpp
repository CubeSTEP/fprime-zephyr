// ======================================================================
// \title Os/Zephyr/Directory.cpp
// \brief zephyr implementation for Os::Directory
// ======================================================================
#include <Fw/Types/Assert.hpp>
#include "fprime-zephyr/Os/Directory.hpp"
#include "fprime-zephyr/Os/error.hpp"
#include <zephyr/fs/fs.h>
#include <zephyr/kernel.h>

namespace Os {
namespace Zephyr {
namespace Directory {

ZephyrDirectory::Status ZephyrDirectory::open(const char* path, OpenMode mode) {
    if(path == nullptr) {
        return NOT_DIR;
    }

    struct fs_dir_t *dir = reinterpret_cast<struct fs_dir_t *>(k_malloc(sizeof(struct fs_dir_t)));
    if(dir == nullptr) {
        return OTHER_ERROR;
    }

    Status status = OP_OK;
    if(mode == OpenMode::CREATE_IF_MISSING || mode == OpenMode::CREATE_EXCLUSIVE) {
        int mkdir_res = fs_mkdir(path);

        if(mkdir_res < 0) {
            if(mode == CREATE_IF_MISSING && mkdir_res == -EEXIST) {

            }
            else {
                k_free(dir);
                dir = nullptr;
                status = Os::Zephyr::errno_to_directory_status(-mkdir_res);
                return status;
            }
        }
    }

    int opendir_res = fs_opendir(dir, path);

    if(opendir_res < 0) {
        k_free(dir);
        dir = nullptr;
        status = Os::Zephyr::errno_to_directory_status(-opendir_res);
    }
    else {
        this->m_handle.m_dir_ptr = dir;
    }

    return status;
}

ZephyrDirectory::Status ZephyrDirectory::rewind() {
    return Status::NOT_SUPPORTED;
}

ZephyrDirectory::Status ZephyrDirectory::read(char * fileNameBuffer, FwSizeType bufSize) {
    return Status::NOT_SUPPORTED;
}

void ZephyrDirectory::close() {
    // no-op
}

DirectoryHandle* ZephyrDirectory::getHandle() {
    return &this->m_handle;
}

} // namespace Directory
} // namespace Zephyr
} // namespace Os