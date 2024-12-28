#ifndef MMU_HPP
#define MMU_HPP

#define VIRT_KERNEL_MASK 0x0 // 0xFFFF000000000000

#define PHYS_MEMORY_SIZE 0x40000000 // 1GB

#define TABLE_SHIFT 9
#define PAGE_SHIFT 12
#define SECTION_SHIFT (PAGE_SHIFT + TABLE_SHIFT)

#define TABLE_SIZE (1 << TABLE_SHIFT)
#define PAGE_SIZE (1 << PAGE_SHIFT)
#define SECTION_SIZE (1 << SECTION_SHIFT)

#define PGD_SHIFT (PAGE_SHIFT + (3 * TABLE_SHIFT))
#define PUD_SHIFT (PAGE_SHIFT + (2 * TABLE_SHIFT))
#define PMD_SHIFT (PAGE_SHIFT + (1 * TABLE_SHIFT))

#define PGD_SIZE                                                               \
  (3 * PAGE_SIZE) // The size of PGD, PUD, and PMD tables when using section
                  // mappings. We don't need PTE table since we are only mapping
                  // 1GB of memory.

#define MM_TYPE_PAGE_TABLE 3
#define MM_TYPE_PAGE 3
#define MM_TYPE_BLOCK 1
#define MM_ACCESS_FLAG (1 << 10)
#define MM_ACCESS_EL0 (1 << 6)

#define MT_DEVICE_nGnRnE 0x0
#define MT_NORMAL_NC 0x1
#define MT_DEVICE_nGnRnE_FLAGS 0x00
#define MT_NORMAL_NC_FLAGS 0x44
#define MAIR_VALUE                                                             \
  (MT_DEVICE_nGnRnE_FLAGS << (8 * MT_DEVICE_nGnRnE)) |                         \
      (MT_NORMAL_NC_FLAGS << (8 * MT_NORMAL_NC))

#define MMU_FLAGS (MM_TYPE_BLOCK | (MT_NORMAL_NC << 2) | MM_ACCESS_FLAG)
#define MMU_DEVICE_FLAGS                                                       \
  (MM_TYPE_BLOCK | (MT_DEVICE_nGnRnE << 2) | MM_ACCESS_FLAG)

#define TCR_T0SZ (64 - 48)
#define TCR_T1SZ ((64 - 48) << 16)
#define TCR_TG0_4K (0 << 14)
#define TCR_TG1_4K (2 << 30)
#define TCR_VALUE (TCR_T0SZ | TCR_T1SZ | TCR_TG0_4K | TCR_TG1_4K)

#endif // MMU_HPP