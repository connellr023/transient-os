#ifndef SYS_REGISTERS_HPP
#define SYS_REGISTERS_HPP

#define SCTLR_RESERVED ((3 << 28) | (3 << 22) | (1 << 20) | (1 << 11))
#define SCTLR_EE_LITTLE_ENDIAN (0 << 25)
#define SCTLR_EOE_LITTLE_ENDIAN (0 << 24)
#define SCTLR_I_CACHE_DISABLED (0 << 12)
#define SCTLR_D_CACHE_DISABLED (0 << 2)
#define SCTLR_MMU_DISABLED (0 << 0)
#define SCTLR_MMU_ENABLED (1 << 0)

#define SCTLR_EL1_VALUE                                                        \
  (SCTLR_RESERVED | SCTLR_EE_LITTLE_ENDIAN | SCTLR_I_CACHE_DISABLED |          \
   SCTLR_D_CACHE_DISABLED | SCTLR_MMU_DISABLED)

#define HCR_EL2_VALUE ((1 << 31) | (1 << 1))

#define SPSR_EL2_VALUE 0x3C4

#endif // SYS_REGISTERS_HPP