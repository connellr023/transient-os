#ifndef MEMORY_HPP
#define MEMORY_HPP

#define PAGE_SHIFT 12
#define TABLE_SHIFT 9
#define SECTION_SHIFT (PAGE_SHIFT + TABLE_SHIFT)

#define PAGE_SIZE (1 << PAGE_SHIFT)
#define SECTION_SIZE (1 << SECTION_SHIFT)

#define LOW_MEMORY (2 * SECTION_SIZE)

#ifndef __ASSEMBLER__

#include <stdint.h>

namespace kernel::memory {
constexpr uint64_t low_memory = LOW_MEMORY;
constexpr uint64_t high_memory = 0x3F000000;
constexpr uint64_t paging_memory = high_memory - low_memory;
constexpr uint64_t page_size = PAGE_SIZE;
constexpr uint64_t page_count = paging_memory / page_size;

uint64_t palloc();
void pfree(uint64_t page);
} // namespace kernel::memory

#endif // __ASSEMBLER__

#endif // MEMORY_HPP