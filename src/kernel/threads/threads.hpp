#ifndef THREADS_HPP
#define THREADS_HPP

namespace kernel::threads {
typedef void (*thread_handler_t)(void *);

void thread_return_handler();
} // namespace kernel::threads

#endif // THREADS_HPP