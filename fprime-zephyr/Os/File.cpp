// ======================================================================
// \title fprime-zephyr/Os/File.cpp
// \brief zephyr implementation for Os::File
// ======================================================================
// #include <Fw/Types/Assert.hpp>
#include "fprime-zephyr/Os/File.hpp"


namespace Os {
namespace Zephyr {
namespace File {

    ZephyrFile::Status ZephyrFile::open(const char *filepath, ZephyrFile::Mode open_mode, OverwriteType overwrite) {
        Status status = Status::NOT_SUPPORTED;
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