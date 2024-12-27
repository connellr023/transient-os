#ifndef VIRTUAL_MEMORY_HPP
#define VIRTUAL_MEMORY_HPP

#define TABLE_GRANULE_SIZE 0b11 // 4KB
#define TABLE_BLOCK_SIZE 0b01   // 2MB

#define MAX_ENTRIES_PER_TABLE 512 // For a 4KB page size

#define ACCESS_EL0 (1 << 6) // EL0 (user) can access
#define ACCESS_EL1 (0 << 6) // EL1 (kernel) can access
#define ACCESS_READ_WRITE (0 << 7)
#define ACCESS_READ_ONLY (1 << 7)
#define ACCESS_ACCESS_FLAG (1 << 10)
#define ACCESS_NO_EXECUTE (1 << 54)

#define SHARABILITY_OUTER (2 << 8) // Outer shareable
#define SHARABILITY_INNER (3 << 8) // Inner shareable

#define REGION_NORAML (0 << 2)        // Normal memory
#define REGION_DEVICE (1 << 2)        // Device MMIO
#define REGION_NON_CACHEABLE (2 << 2) // Non-cacheable

#define TTBR_CNP 1 // Translation tables are not unique to current CPU core

namespace kernel::memory {
void init_virtual_memory() asm("_init_virtual_memory");
}

#endif // VIRTUAL_MEMORY_HPP