#ifndef MMU_HPP
#define MMU_HPP

#include <kernel/peripherals/mmio.hpp>

#define PAGE_SHIFT 12 // 4KB page size
#define TABLE_SHIFT 9
#define SECTION_SHIFT (PAGE_SHIFT + TABLE_SHIFT)

#define PAGE_SIZE (1 << PAGE_SHIFT)
#define SECTION_SIZE (1 << SECTION_SHIFT)

#define LOW_MEMORY (2 * SECTION_SIZE)
#define HIGH_MEMORY MMIO_BASE

#define PAGING_MEMORY (HIGH_MEMORY - LOW_MEMORY)
#define PAGE_COUNT (PAGING_MEMORY / PAGE_SIZE)

#define TABLE_GRANULE 0b11 // 4KB
#define TABLE_BLOCK 0b01   // 2MB

#define MAX_ENTRIES_PER_TABLE 512 // For a 4KB page size

#define ACCESS_EL0 (1 << 6) // EL0 (user) can access
#define ACCESS_EL1 (0 << 6) // EL1 (kernel) can access
#define ACCESS_READ_WRITE (0 << 7)
#define ACCESS_READ_ONLY (1 << 7)
#define ACCESS_ACCESS_FLAG (1 << 10)
#define ACCESS_NO_EXECUTE (1 << 54)

#define SHARABILITY_OUTER (2 << 8) // Outer shareable
#define SHARABILITY_INNER (3 << 8) // Inner shareable

#define REGION_NORMAL (0 << 2)        // Normal memory
#define REGION_DEVICE (1 << 2)        // Device MMIO
#define REGION_NON_CACHEABLE (2 << 2) // Non-cacheable

#endif // MMU_HPP