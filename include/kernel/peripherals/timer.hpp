/*
 * Copyright (C) 2024/2025 connellr023@github
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

#ifndef TIMER_INTERRUPT_HPP
#define TIMER_INTERRUPT_HPP

#include <kernel/peripherals/mmio.hpp>
#include <stdint.h>

#define TIMER_CS ((volatile uint32_t *)(MMIO_BASE + 0x00003000))
#define TIMER_COUNTER_LOW ((volatile uint32_t *)(MMIO_BASE + 0x00003004))
#define TIMER_COUNTER_HIGH ((volatile uint32_t *)(MMIO_BASE + 0x00003008))

#define TIMER_CMP_1 ((volatile uint32_t *)(MMIO_BASE + 0x00003010))
#define TIMER_CS_M1 (1 << 1)

#define ENABLE_IRQS_1 ((volatile uint32_t *)(MMIO_BASE + 0x0000B210))
#define SYSTEM_TIMER_IRQ_1 (1 << 1)

#endif // TIMER_INTERRUPT_HPP