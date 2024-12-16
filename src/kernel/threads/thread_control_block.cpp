#include "thread_control_block.hpp"
#include "../memory/paging.hpp"

using namespace kernel::threads;

uint64_t thread_id_counter = 0;

ThreadControlBlock::ThreadControlBlock(thread_handler_t handler,
                                       uint32_t burst_time, void *arg)
    : thread_id(thread_id_counter++), page_addr(0), burst_time(burst_time),
      state(ThreadState::Uninitialized), sp(0), handler(handler), arg(arg) {}