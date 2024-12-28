#include <kernel/memory/paging.hpp>
#include <kernel/memory/virtual_memory.hpp>
#include <kernel/sys/sys_registers.hpp>

alignas(PAGE_SIZE) uint64_t pgd_table[PGD_ELEMENTS] = {0};

namespace kernel::memory {
void init_virtual_memory() {}
} // namespace kernel::memory