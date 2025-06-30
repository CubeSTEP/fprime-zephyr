// ======================================================================
// \title Os/Zephyr/Directory.cpp
// \brief zephyr implementation for Os::Directory
// ======================================================================
#include "fprime-zephyr/Os/Directory.hpp"

namespace Os {
namespace Zephyr {
namespace Directory {

ZephyrDirectory::Status ZephyrDirectory::open(const char* path, OpenMode mode) {
    return Status::NOT_SUPPORTED;
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