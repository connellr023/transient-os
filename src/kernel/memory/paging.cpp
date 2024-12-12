#include "paging.hpp"

using namespace kernel::memory;

bool memory_map[PAGE_COUNT] = {false};

void *kernel::memory::palloc() {
  for (uint64_t i = 0; i < PAGE_COUNT; i++) {
    if (!memory_map[i]) {
      memory_map[i] = true;

      return reinterpret_cast<void *>(SP_EL1_LOW_MEMORY + (i * PAGE_SIZE));
    }
  }

  return nullptr;
}

void kernel::memory::pfree(void *page) {
  const uint64_t page_addr = reinterpret_cast<uint64_t>(page);
  memory_map[(page_addr - SP_EL1_LOW_MEMORY) / PAGE_SIZE] = false;
}