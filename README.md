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

## Threads

Threads are the fundamental unit of execution in this operating system. Each thread is allocated a single 4KB page of memory and is scheduled by the kernel. Threads are not tied to each other, that is, there is no concept of parent and child threads. All threads (except the main startup thread) are created by the user and are managed by the kernel.

## System Calls

It is worth noting that everytime a system call is made, the scheduler preempts the callee thread and it will have to wait throught the entire round-robin cycle before it can be scheduled again. This means system calls are blocking and expensive.

### Set Output Handler (`set_output_handler`)

This call sets the debug output handler for the kernel. The output handler is a function that takes a `const char*` and writes it to the output device. This is intended to be used for debugging purposes.

### Put String (`put_string`)

This call writes a message to the output handler. The output handler must have been initialized, or nothing will happen. This is intended as more of a test system call than anything else but can be used to safely transmit over **UART** if output handler is configured to do so.

### Heap Allocation (`heap_alloc`)

This call allocates a block of memory on the heap and returns a pointer to the allocated block. If no memory is available, it returns `nullptr`. This is analogous to `malloc` in C.

### Heap Free (`heap_free`)

This call frees a previously allocated block of memory on the heap.

### Heap Realloc (`heap_realloc`)

This call reallocates a previously allocated block of memory on the heap.

### Yield (`yield`)

This call yields the current thread, allowing the scheduler to switch to another thread.

### Exit (`exit`)

This call exits the current thread and cleans up any resources it was using.

### Sleep (`sleep`)

This call sleeps for at least the specified number of microseconds before resuming execution.

### Spawn Thread (`spawn_thread`)

This call spawns a new thread and populates a `ThreadHandle` that can be used to join the thread later.

## System Call Protocol

The system call protocol is as follows:

- The system call code is passed into the `w8` register.
- A single system call argument is passed into the `x0` register. (This can be a pointer to a structure to pass multiple arguments)
- The system call is executed by calling the `svc` instruction.

The API provided abstracts this protocol away from developers.

## Usage

When writing code that utilizes this operating system, it is intended that you only include header files from `include/api` as these are the only files that are intended to be used by the user. The `include/kernel` directory is for internal kernel use only.

## High-Level Design

The operating system is intended to be simple but useful. It is designed to be a time-sharing operating system that can run multiple threads concurrently. The system is designed to run on a single core and all threads run at EL1 (as of now). The system is designed to be preemptive and uses round-robin scheduling with configurable time quantums for each thread.

### Thread Memory Layout

Each thread is allocated a single 4KB page of memory. The memory layout for each thread is described below.

#### Stack

The stack is located at the top of the memory page and grows downwards. The stack is intended to be half the size of the memory page, but this is not enforced and can certainly overflow into the heap.

#### Heap

The heap is located at the bottom of the memory page and grows upwards. The heap is constrained to be the other half of the memory page and cannot grow beyond that. The heap is implemented as a first-fit allocator with a simple free list. The heap is intended to be used for dynamic memory allocation but is prone to fragmentation.

## Diagrams (as of now)

<div align="center">
  <img src="https://github.com/connellr023/connellr023.github.io/blob/main/static/images/transient-os/memory-layout.png?raw=true" alt="Memory Layout" width="500"/>
  <img src="https://github.com/connellr023/connellr023.github.io/blob/main/static/images/transient-os/thread-heap-memory-layout.png?raw=true" alt="Thread Heap Memory Layout" width="400"/>
</div>

## Development

Developed and tested by [connellr023](https://github.com/connellr023) in 2024/2025.

## License

This project is licensed under the **MIT License**.
