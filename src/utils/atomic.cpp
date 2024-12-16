#include "atomic.hpp"
#include "../kernel/interrupts/interrupts.hpp"

using namespace kernel::interrupts;

AtomicBlock::AtomicBlock() { disable_interrupts(); }
AtomicBlock::~AtomicBlock() { enable_interrupts(); }