#ifndef INCLUDE_KERNEL_UTIL_CONSTANTS_H_
#define INCLUDE_KERNEL_UTIL_CONSTANTS_H_

#include "kernel/util/stdint.h"

constexpr uint64_t NUM_CORES                      = 4;

// peripherals base address
constexpr size_t PBASE                          = 0x3F000000;

// GPIO register addresses
constexpr size_t GPFSEL1                        = (PBASE+0x00200004);
constexpr size_t GPSET0                         = (PBASE+0x0020001C);
constexpr size_t GPCLR0                         = (PBASE+0x00200028);
constexpr size_t GPPUD                          = (PBASE+0x00200094);
constexpr size_t GPPUDCLK0                      = (PBASE+0x00200098);

// UART register addresses
constexpr size_t AUX_ENABLES                    = (PBASE+0x00215004);
constexpr size_t AUX_MU_IO_REG                  = (PBASE+0x00215040);
constexpr size_t AUX_MU_IER_REG                 = (PBASE+0x00215044);
constexpr size_t AUX_MU_IIR_REG                 = (PBASE+0x00215048);
constexpr size_t AUX_MU_LCR_REG                 = (PBASE+0x0021504C);
constexpr size_t AUX_MU_MCR_REG                 = (PBASE+0x00215050);
constexpr size_t AUX_MU_LSR_REG                 = (PBASE+0x00215054);
constexpr size_t AUX_MU_MSR_REG                 = (PBASE+0x00215058);
constexpr size_t AUX_MU_SCRATCH                 = (PBASE+0x0021505C);
constexpr size_t AUX_MU_CNTL_REG                = (PBASE+0x00215060);
constexpr size_t AUX_MU_STAT_REG                = (PBASE+0x00215064);
constexpr size_t AUX_MU_BAUD_REG                = (PBASE+0x00215068);

// SCTLR_EL1, System Control Register (EL1), Page 2654 of AArch64-Reference-Manual.
constexpr size_t SCTLR_RESERVED                 = (3 << 28) | (3 << 22) | (1 << 20) | (1 << 11);
constexpr size_t SCTLR_EE_LITTLE_ENDIAN         = (0 << 25);
constexpr size_t SCTLR_EOE_LITTLE_ENDIAN        = (0 << 24);
constexpr size_t SCTLR_I_CACHE_DISABLED         = (0 << 12);
constexpr size_t SCTLR_D_CACHE_DISABLED         = (0 << 2);
constexpr size_t SCTLR_MMU_DISABLED             = (0 << 0);
constexpr size_t SCTLR_MMU_ENABLED              = (1 << 0);
constexpr size_t SCTLR_VALUE_MMU_DISABLED       = (SCTLR_RESERVED | SCTLR_EE_LITTLE_ENDIAN | SCTLR_I_CACHE_DISABLED | SCTLR_D_CACHE_DISABLED | SCTLR_MMU_DISABLED);

// HCR_EL2, Hypervisor Configuration Register (EL2), Page 2487 of AArch64-Reference-Manual.
constexpr size_t HCR_RW                         = (1 << 31);
constexpr size_t HCR_VALUE                      = HCR_RW;

// SCR_EL3, Secure Configuration Register (EL3), Page 2648 of AArch64-Reference-Manual.
constexpr size_t SCR_RESERVED                   = (3 << 4);
constexpr size_t SCR_RW                         = (1 << 10);
constexpr size_t SCR_NS                         = (1 << 0);
constexpr size_t SCR_VALUE                      = (SCR_RESERVED | SCR_RW | SCR_NS);

// SPSR_EL3, Saved Program Status Register (EL3) Page 389 of AArch64-Reference-Manual.
constexpr size_t SPSR_MASK_ALL                  = (7 << 6);
constexpr size_t SPSR_EL1h                      = (5 << 0);
constexpr size_t SPSR_VALUE                     = (SPSR_MASK_ALL | SPSR_EL1h);

#endif  // INCLUDE_KERNEL_UTIL_CONSTANTS_H_
