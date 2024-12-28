#include <kernel/memory/mmu.hpp>
#include <kernel/memory/paging.hpp>
#include <kernel/memory/virtual_memory.hpp>
#include <kernel/sys/sys_registers.hpp>
#include <stdint.h>

/**
 * @brief Single-level page table for all memory
 */
alignas(PAGE_SIZE) uint64_t translation_table[MAX_ENTRIES_PER_TABLE] = {0};

namespace kernel::memory {
void init_virtual_memory() {
  // Map the first 1 GB of physical memory to virtual memory 1:1
  translation_table[0] = 0x0 | TABLE_BLOCK | ACCESS_ACCESS_FLAG |
                         SHARABILITY_INNER | REGION_NORMAL | ACCESS_READ_WRITE |
                         ACCESS_EL1;

  // Set up the system registers
  uint64_t ttbr0 = reinterpret_cast<uint64_t>(translation_table);
  asm volatile("msr ttbr0_el1, %0" : : "r"(ttbr0));
  asm volatile("dsb ish"); // Ensure completion of the write
  asm volatile("isb");     // Synchronize context on this processor

  // first, set Memory Attributes array, indexed by PT_MEM, PT_DEV, PT_NC in our
  // example
  uint64_t r = (0xFF << 0) | // AttrIdx=0: normal, IWBWA, OWBWA, NTR
               (0x04 << 8) | // AttrIdx=1: device, nGnRE (must be OSH too)
               (0x44 << 16); // AttrIdx=2: non cacheable

  asm volatile("msr mair_el1, %0" : : "r"(r));

  // next, specify mapping characteristics in translate control register
  r = (0b00LL << 37) | // TBI=0, no tagging
      (0b10LL << 30) | // TG1=4k
      (0b11LL << 28) | // SH1=3 inner
      (0b01LL << 26) | // ORGN1=1 write back
      (0b01LL << 24) | // IRGN1=1 write back
      (0b0LL << 23) |  // EPD1 enable higher half
      (25LL << 16) |   // T1SZ=25, 3 levels (512G)
      (0b00LL << 14) | // TG0=4k
      (0b11LL << 12) | // SH0=3 inner
      (0b01LL << 10) | // ORGN0=1 write back
      (0b01LL << 8) |  // IRGN0=1 write back
      (0b0LL << 7) |   // EPD0 enable lower half
      (25LL << 0);     // T0SZ=25, 3 levels (512G)
  asm volatile("msr tcr_el1, %0; isb" : : "r"(r));

  asm volatile("msr ttbr0_el1, %0" : : "r"(&translation_table));

  // finally, toggle some bits in system control register to enable page
  // translation
  asm volatile("dsb ish; isb; mrs %0, sctlr_el1" : "=r"(r));
  r |= 0xC00800;     // set mandatory reserved bits
  r &= ~((1 << 25) | // clear EE, little endian translation tables
         (1 << 24) | // clear E0E
         (1 << 19) | // clear WXN
         (1 << 12) | // clear I, no instruction cache
         (1 << 4) |  // clear SA0
         (1 << 3) |  // clear SA
         (1 << 2) |  // clear C, no cache at all
         (1 << 1));  // clear A, no aligment check
  r |= (1 << 0);     // set M, enable MMU
  asm volatile("msr sctlr_el1, %0; isb" : : "r"(r));
}
} // namespace kernel::memory