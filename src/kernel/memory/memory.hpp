#ifndef MEMORY_HPP
#define MEMORY_HPP

#define PAGE_SHIFT 12
#define TABLE_SHIFT 9
#define SECTION_SHIFT (PAGE_SHIFT + TABLE_SHIFT)

#define PAGE_SIZE (1 << PAGE_SHIFT)
#define SECTION_SIZE (1 << SECTION_SHIFT)

#define LOW_MEMORY (2 * SECTION_SIZE)
#define HIGH_MEMORY 0x3F000000

#define PAGING_MEMORY (HIGH_MEMORY - LOW_MEMORY)
#define PAGE_COUNT (PAGING_MEMORY / PAGE_SIZE)

#ifndef __ASSEMBLER__

#include <stdint.h>

namespace kernel::memory {
uint64_t palloc();
void pfree(uint64_t page);
} // namespace kernel::memory

#endif // __ASSEMBLER__

#endif // MEMORY_HPP