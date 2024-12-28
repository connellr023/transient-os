#include <kernel/memory/mmu.hpp>
#include <kernel/peripherals/mmio.hpp>

.section .text

.macro create_pgd_entry, tbl, virt, tmp1, tmp2
    create_table_entry \tbl, \virt, PGD_SHIFT, \tmp1, \tmp2
    create_table_entry \tbl, \virt, PUD_SHIFT, \tmp1, \tmp2
.endm

.macro create_table_entry, tbl, virt, shift, tmp1, tmp2
    lsr \tmp1, \virt, #\shift
    and \tmp1, \tmp1, #(TABLE_SIZE-1)// table index
    add \tmp2, \tbl, #PAGE_SIZE
    orr \tmp2, \tmp2, #MM_TYPE_PAGE_TABLE
    str \tmp2, [\tbl, \tmp1, lsl#3]
    add \tbl, \tbl, #PAGE_SIZE // next level table page
.endm

.macro create_block_map, tbl, phys, start, end, flags, tmp1
    lsr \start, \start, #SECTION_SHIFT
    and \start, \start, #(TABLE_SIZE-1)// table index
    lsr \end, \end, #SECTION_SHIFT
    and \end, \end, #(TABLE_SIZE-1)// table end index
    lsr \phys, \phys, #SECTION_SHIFT
    mov \tmp1, \flags
    orr \phys, \tmp1, \phys, lsl #SECTION_SHIFT // table entry
9999:
    str \phys, [\tbl, \start, lsl#3]                                    // store the entry
    add \start, \start, #1                                              // next entry
    add \phys, \phys, #SECTION_SIZE                                     // next block
    cmp \start, \end
    b.ls 9999b
.endm

.globl _create_page_tables
_create_page_tables:
    mov x29, x30 // Save the link register

    adrp x0, pgd_table // Defined in virtual_memory.hpp
    mov x1, #VIRT_KERNEL_MASK
    create_pgd_entry x0, x1, x2, x3

    // Map kernel and init stack
    mov x1, xzr
    ldr x2, =VIRT_KERNEL_MASK
    ldr x3, =(VIRT_KERNEL_MASK+MMIO_BASE-SECTION_SIZE)
    ldr x4, =MMU_FLAGS
    create_block_map x0, x1, x2, x3, x4, x5

    // Map peripherals
    ldr x1, =MMIO_BASE
    ldr x2, =(VIRT_KERNEL_MASK+MMIO_BASE)
    ldr x3, =(VIRT_KERNEL_MASK+PHYS_MEMORY_SIZE-SECTION_SIZE)
    ldr x4, =MMU_DEVICE_FLAGS
    create_block_map x0, x1, x2, x3, x4, x5

    mov x30, x29 // Restore the link register
    ret

.globl _init_page_tables
_init_page_tables:
    ldr x0, =pgd_table
    msr ttbr1_el1, x0
    ret
