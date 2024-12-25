# $\text{Transient OS}$

> A work in progress _time-sharing_ operating system for concurrency on the **Raspberry Pi 3**.

![Raspberry Pi](https://img.shields.io/badge/-Raspberry_Pi-C51A4A?style=for-the-badge&logo=Raspberry-Pi)
![C++](https://img.shields.io/badge/c++-%2300599C.svg?style=for-the-badge&logo=cplusplus&logoColor=white)
![ARMv8](https://img.shields.io/badge/ARMv8-%23000000.svg?style=for-the-badge&logo=arm&logoColor=00c1de)

## Overview

$\text{Transient OS}$ is a time-sharing operating system designed for the Raspberry Pi 3. It uses preemptive round-robin scheduling with configurable time quantums to manage concurrency. The system includes several system calls for memory management and thread control.

## Example

A project showcasing the capability of this project can be found <a href="https://github.com/connellr023/game-of-life-tos">here</a>. This showcase project is **Conway's Game of Life** where each cell is a thread that runs concurrently.

## Purpose

This operating system is intended to still be used for very low-level programming somewhat similar to a real-time operating system. Additionally, it was also created to learn more about operating systems and how they work.

## Features

- **Preemptive Round-Robin Scheduling**: Ensures fair CPU time distribution among threads with configurable time quantums.
- **System Calls**: Provides essential system calls for memory management and thread control.
- **Memory Management**: No virtual memory with paging and heap allocation.
- **Thread Management**: All threads, including the kernel, run at EL1. All threads run on a single core.
- **Utilities**:
  - `Mutex`: Class that provides a way to create mutual exclusion locks.
  - `MutexLockGuard`: Class that provides a way to create mutual exclusion blocks of code.
  - `AtomicGuard`: Class that provides a way to create atomic blocks of code. (Likely will be removed in future versions)

## System Calls

It is worth noting that everytime a system call is made, the scheduler preempts the callee thread and it will have to wait throught the entire round-robin cycle before it can be scheduled again. This means system calls are blocking and expensive.

### Put String (`put_string`)

This call writes a message to the output handler. The output handler must have been initialized, or nothing will happen. This is intended as more of a test system call than anything else but can be used to safely transmit over **UART** if output handler is configured to do so.

### Heap Allocation (`heap_alloc`)

This call allocates a block of memory on the heap and returns a pointer to the allocated block. If no memory is available, it returns `nullptr`. This is analogous to `malloc` in C.

### Heap Free (`heap_free`)

This call frees a previously allocated block of memory on the heap.

### Yield (`yield`)

This call yields the current thread, allowing the scheduler to switch to another thread.

### Exit (`exit`)

This call exits the current thread and cleans up any resources it was using.

### Sleep (`sleep`)

This call sleeps for at least the specified number of microseconds before resuming execution.

### Spawn Thread (`spawn_thread`)

This call spawns a new thread from a `ThreadControlBlock` and starts executing it.

### Diagrams (as of now)

![Memory Layout](https://github.com/connellr023/connellr023.github.io/blob/main/static/images/transient-os/memory-layout.png?raw=true)

![Thread Heap Memory Layout](https://github.com/connellr023/connellr023.github.io/blob/main/static/images/transient-os/thread-heap-memory-layout.png?raw=true)

## Development

Developed and tested by [connellr023](https://github.com/connellr023) in 2024/2025.

## License

This project is licensed under the **MIT License**.
