// ======================================================================
// \title Os/Zephyr/File.cpp
// \brief zephyr implementation for Os::File
// ======================================================================
#include "fprime-zephyr/Os/FileSystem.hpp"

namespace Os {
namespace Zephyr {
namespace FileSystem {

ZephyrFileSystem::Status ZephyrFileSystem::_removeDirectory(const char* path) {
    return Status::NOT_SUPPORTED;
}

ZephyrFileSystem::Status ZephyrFileSystem::_removeFile(const char* path) {
    return Status::NOT_SUPPORTED;
}

ZephyrFileSystem::Status ZephyrFileSystem::_rename(const char* originPath, const char* destPath) {
    return Status::NOT_SUPPORTED;
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