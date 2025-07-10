// ======================================================================
// \title Os/Posix/error.hpp
// \brief header for posix errno conversion
// ======================================================================
#ifndef OS_ZEPHYR_ERRNO_HPP
#define OS_ZEPHYR_ERRNO_HPP
#include "Os/File.hpp"
#include "Os/FileSystem.hpp"
#include "Os/Directory.hpp"

namespace Os {
namespace Zephyr {

//! Convert an errno representation of an error to the Os::File::Status representation.
//! \param errno_input: errno representation of the error
//! \return: Os::File::Status representation of the error
//!
Os::File::Status errno_to_file_status(int errno_input);

//! Convert an errno representation of an error to the Os::FileSystem::Status representation.
//! \param errno_input: errno representation of the error
//! \return: Os::FileSystem::Status representation of the error
//!
Os::FileSystem::Status errno_to_filesystem_status(int errno_input);

//! Convert an errno representation of an error to the Os::FileSystem::Status representation.
//! \param errno_input: errno representation of the error
//! \return: Os::Directory::Status representation of the error
//!
Os::Directory::Status errno_to_directory_status(int errno_input);

}
}
#endif