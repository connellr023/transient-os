#ifndef MEMORY_HPP
#define MEMORY_HPP

#include "../../drivers/gpio.hpp"

#define PAGE_SHIFT 11
#define TABLE_SHIFT 10
#define SECTION_SHIFT (PAGE_SHIFT + TABLE_SHIFT)

#define PAGE_SIZE (1 << PAGE_SHIFT)
#define SECTION_SIZE (1 << SECTION_SHIFT)

#define LOW_MEMORY (1 * SECTION_SIZE)
#define HIGH_MEMORY MMIO_BASE

#define PAGING_MEMORY (HIGH_MEMORY - LOW_MEMORY)
#define PAGE_COUNT (PAGING_MEMORY / PAGE_SIZE)

#ifndef __ASSEMBLER__

#include <stdint.h>

namespace kernel::memory {
void *palloc();
void pfree(void *page);
} // namespace kernel::memory

#endif // __ASSEMBLER__

#endif // MEMORY_HPP