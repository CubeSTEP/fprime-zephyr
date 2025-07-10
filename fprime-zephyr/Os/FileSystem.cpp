// ======================================================================
// \title Os/Zephyr/File.cpp
// \brief zephyr implementation for Os::File
// ======================================================================
#include "fprime-zephyr/Os/FileSystem.hpp"
#include <zephyr/fs/fs.h>

namespace Os {
namespace Zephyr {
namespace FileSystem {

ZephyrFileSystem::Status ZephyrFileSystem::_removeDirectory(const char* path) {
    Status status = OP_OK;
    int unlink_res = fs_unlink(path);

    if(unlink_res < 0) {
        status = NOT_SUPPORTED; // TODO
    }
    return status;
}

ZephyrFileSystem::Status ZephyrFileSystem::_removeFile(const char* path) {
    return this->_removeDirectory(path);
}

ZephyrFileSystem::Status ZephyrFileSystem::_rename(const char* originPath, const char* destPath) {
    Status status = OP_OK;
    int unlink_res = fs_rename(originPath, destPath);

    if(unlink_res < 0) {
        status = NOT_SUPPORTED; // TODO
    }
    return status;
}

ZephyrFileSystem::Status ZephyrFileSystem::_getWorkingDirectory(char* path, FwSizeType bufferSize) {
    return Status::NOT_SUPPORTED;
}

ZephyrFileSystem::Status ZephyrFileSystem::_changeWorkingDirectory(const char* path) {
    return Status::NOT_SUPPORTED;
}

ZephyrFileSystem::Status ZephyrFileSystem::_getFreeSpace(const char* path, FwSizeType& totalBytes, FwSizeType& freeBytes) {
    return Status::NOT_SUPPORTED;
}

FileSystemHandle* ZephyrFileSystem::getHandle() {
    return &this->m_handle;
}

ZephyrFileSystem::Status ZephyrFileSystem::_getPathType(const char* path, PathType& pathType) {
    return Status::NOT_SUPPORTED;
}

} // namespace FileSystem
} // namespace Zephyr
} // namespace Os