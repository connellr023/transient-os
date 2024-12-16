#ifndef ATOMIC_BLOCK_HPP
#define ATOMIC_BLOCK_HPP

// RAII class for atomic block
class AtomicBlock {
public:
  inline AtomicBlock();
  inline ~AtomicBlock();
};

#endif // ATOMIC_BLOCK_HPP