// ======================================================================
// \title Os/Posix/DefaultRawTime.cpp
// \brief sets default Os::RawTime Posix implementation via linker
// ======================================================================
#include "Os/Delegate.hpp"
#include "fprime-zephyr/Os/RawTime.hpp"

namespace Os {

//! \brief get a delegate for RawTimeInterface that intercepts calls for Posix
//! \param aligned_new_memory: aligned memory to fill
//! \param to_copy: pointer to copy-constructor input
//! \return: pointer to delegate
RawTimeInterface *RawTimeInterface::getDelegate(RawTimeHandleStorage& aligned_new_memory, const RawTimeInterface* to_copy) {
    return Os::Delegate::makeDelegate<RawTimeInterface, Os::Zephyr::RawTime::ZephyrRawTime, RawTimeHandleStorage>(
            aligned_new_memory, to_copy
    );
}

}  // namespace Os