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

#include <kernel/interrupts/interrupts.hpp>
#include <kernel/peripherals/timer.hpp>

namespace kernel::interrupts {
void clear_timer_interrupt() { *TIMER_CS = TIMER_CS_M1; }

void prepare_timer_interrupt(uint32_t interval_us) {
  static uint32_t current_us = 0;

  current_us = *TIMER_COUNTER_LOW;
  *TIMER_CMP_1 = current_us + interval_us;
}

void enable_interrupt_controller() {
  // Enable the system timer IRQ
  *ENABLE_IRQS_1 = SYSTEM_TIMER_IRQ_1;
}
} // namespace kernel::interrupts