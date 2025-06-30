// ======================================================================
// \title Os/Zephyr/DefaultFile.cpp
// \brief sets default Os::File to no-op zephyr implementation via linker
// ======================================================================
#include "Os/File.hpp"
#include "fprime-zephyr/Os/File.hpp"
#include "Os/FileSystem.hpp"
#include "fprime-zephyr/Os/FileSystem.hpp"
#include "Os/Directory.hpp"
#include "fprime-zephyr/Os/Directory.hpp"
#include "Os/Delegate.hpp"
namespace Os {

//! \brief get a delegate for FileInterface that intercepts calls for zephyr file usage
//! \param aligned_new_memory: aligned memory to fill
//! \param to_copy: pointer to copy-constructor input
//! \return: pointer to delegate
FileInterface *FileInterface::getDelegate(FileHandleStorage& aligned_placement_new_memory, const FileInterface* to_copy) {
    return Os::Delegate::makeDelegate<FileInterface, Os::Zephyr::File::ZephyrFile>(
            aligned_placement_new_memory, to_copy
    );
}

//! \brief get a delegate for FileSystemInterface that intercepts calls for zephyr fileSystem usage
//! \param aligned_new_memory: aligned memory to fill
//! \param to_copy: pointer to copy-constructor input
//! \return: pointer to delegate
FileSystemInterface *FileSystemInterface::getDelegate(FileSystemHandleStorage& aligned_placement_new_memory) {
    return Os::Delegate::makeDelegate<FileSystemInterface, Os::Zephyr::FileSystem::ZephyrFileSystem>(
        aligned_placement_new_memory
    );
}

//! \brief get a delegate for DirectoryInterface that intercepts calls for zephyr Directory usage
//! \param aligned_new_memory: aligned memory to fill
//! \return: pointer to delegate
DirectoryInterface *DirectoryInterface::getDelegate(DirectoryHandleStorage& aligned_placement_new_memory) {
    return Os::Delegate::makeDelegate<DirectoryInterface, Os::Zephyr::Directory::ZephyrDirectory>(
        aligned_placement_new_memory
    );
}
}