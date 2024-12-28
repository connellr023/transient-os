#ifndef VIRTUAL_MEMORY_HPP
#define VIRTUAL_MEMORY_HPP

#include <kernel/memory/mmu.hpp>
#include <stdint.h>

#define PGD_ELEMENTS (PGD_SIZE / sizeof(uint64_t))

extern uint64_t pgd_table[PGD_ELEMENTS];

namespace kernel::memory {
void init_virtual_memory();
}

#endif // VIRTUAL_MEMORY_HPP