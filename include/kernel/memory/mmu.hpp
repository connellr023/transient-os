#ifndef MMU_HPP
#define MMU_HPP

#define TABLE_SIZE 512             // 2 ^ (9 bits)
#define PAGE_SIZE (TABLE_SIZE * 8) // 8 bytes (64-bit addresses) per entry (4KB)
#define SECTION_SIZE (TABLE_SIZE * PAGE_SIZE) // 2MB

#define PAGE_GLOBAL_DIRECTORY_SIZE                                             \
  (3 * PAGE_SIZE) // The size of PGD, PUD, and PMD tables when using section
                  // mappings. We don't need PTE table since we are only mapping
                  // 1GB of memory.

// #define ACCESS_EL0 (1 << 6) // EL0 (user) can access
// #define ACCESS_EL1 (0 << 6) // EL1 (kernel) can access
// #define ACCESS_READ_WRITE (0 << 7)
// #define ACCESS_READ_ONLY (1 << 7)
// #define ACCESS_ACCESS_FLAG (1 << 10)
// #define ACCESS_NO_EXECUTE (1 << 54)

// #define SHARABILITY_OUTER (2 << 8) // Outer shareable
// #define SHARABILITY_INNER (3 << 8) // Inner shareable

// #define REGION_NORMAL (0 << 2)        // Normal memory
// #define REGION_DEVICE (1 << 2)        // Device MMIO
// #define REGION_NON_CACHEABLE (2 << 2) // Non-cacheable

#define MAIR_ATTR_DEVICE_nGnRnE 0x0
#define MAIR_ATTR_NORMAL_NC 0x1
#define MAIR_FLAG_DEVICE_nGnRnE 0x0
#define MAIR_FLAG_NORMAL_NC 0x44
#define MAIR_VALUE                                                             \
  (MAIR_FLAG_DEVICE_nGnRnE << (8 * MAIR_ATTR_DEVICE_nGnRnE)) |                 \
      (MAIR_FLAG_NORMAL_NC << (8 * MAIR_ATTR_NORMAL_NC))

#endif // MMU_HPP