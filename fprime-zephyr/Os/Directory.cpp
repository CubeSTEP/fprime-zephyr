// ======================================================================
// \title Os/Zephyr/Directory.cpp
// \brief zephyr implementation for Os::Directory
// ======================================================================
#include <Fw/Types/Assert.hpp>
#include <Fw/Types/StringUtils.hpp>
#include "fprime-zephyr/Os/Directory.hpp"
#include "fprime-zephyr/Os/error.hpp"
#include <zephyr/fs/fs.h>
#include <zephyr/kernel.h>

// Remove later
#include <cstdlib>
#define DEBUG_PRINT(x,...) printk(x,##__VA_ARGS__);

namespace Os {
namespace Zephyr {
namespace Directory {


// Might Need to mount filesystem first? Where should that be done?

ZephyrDirectory::Status ZephyrDirectory::open(const char* path, OpenMode mode) {
    DEBUG_PRINT("TEST: OPEN DIRECTORY START\n");
    if(path == nullptr) {
        DEBUG_PRINT("TEST: OPEN DIRECTORY NOT A PATH\n");
        return NOT_DIR;
    }

    struct fs_dir_t *dir = reinterpret_cast<struct fs_dir_t *>(k_malloc(sizeof(struct fs_dir_t)));
    if(dir == nullptr) {
        DEBUG_PRINT("TEST: OPEN DIRECTORY NOT ENOUGH MEMORY\n");
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
                DEBUG_PRINT("TEST: OPEN DIRECTORY CREATE NEW DIRECTORY ERROR\n");
                return status;
            }
        }
    }

    fs_dir_t_init(dir);
    int opendir_res = fs_opendir(dir, path);

    if(opendir_res < 0) {
        k_free(dir);
        dir = nullptr;
        status = Os::Zephyr::errno_to_directory_status(-opendir_res);
    }
    else {
        size_t path_len = strlen(path) + 1;
        this->m_handle.m_path = (char*)k_malloc(path_len);
        if (this->m_handle.m_path == nullptr) { // Should not being able to store the path cause this function to fail? path is only needed for rewind()
            k_free(dir);
            DEBUG_PRINT("TEST: OPEN DIRECTORY COULD NOT STORE PATH\n");
            return OTHER_ERROR;
        }
        strncpy((char*)this->m_handle.m_path, path, path_len);

        this->m_handle.m_dir_ptr = dir;
    }
    DEBUG_PRINT("TEST: OPEN DIRECTORY SUCCESS\n");
    return status;
}

bool ZephyrDirectory::isOpen() const {
    return this->m_handle.m_dir_ptr != nullptr;
}

ZephyrDirectory::Status ZephyrDirectory::rewind() {
    if (!isOpen() || !this->m_handle.m_path[0]) return NOT_OPENED;

    int closedir_res = fs_closedir(this->m_handle.m_dir_ptr);

    if(closedir_res < 0) {
        return Os::Zephyr::errno_to_directory_status(-closedir_res);
    }

    int opendir_res = fs_opendir(this->m_handle.m_dir_ptr, this->m_handle.m_path);

    if(opendir_res < 0) {
        // Maybe set zdp & path pointers to nullptr and ""
        return Os::Zephyr::errno_to_directory_status(-opendir_res);
    }

    return OP_OK;
}

ZephyrDirectory::Status ZephyrDirectory::read(char * fileNameBuffer, FwSizeType bufSize) {
    FW_ASSERT(fileNameBuffer);
    Status status = Status::OP_OK;

    struct fs_dirent direntData;

    int readdir_res = fs_readdir(this->m_handle.m_dir_ptr, &direntData);

    if(readdir_res < 0) {
        status = Os::Zephyr::errno_to_directory_status(-readdir_res);
    }
    else if(direntData.name[0] == 0) {
        status = NO_MORE_FILES;
    }
    else {
        Fw::StringUtils::string_copy(fileNameBuffer, direntData.name, bufSize);
    }

    return status;
}

void ZephyrDirectory::close() {
    if(!isOpen()) return;

    fs_closedir(this->m_handle.m_dir_ptr);

    k_free(this->m_handle.m_dir_ptr);
    this->m_handle.m_dir_ptr = nullptr;

    if (this->m_handle.m_path != nullptr) {
        k_free((void*)this->m_handle.m_path);
        this->m_handle.m_path = nullptr;
    }
}

DirectoryHandle* ZephyrDirectory::getHandle() {
    return &this->m_handle;
}

} // namespace Directory
} // namespace Zephyr
} // namespace Os