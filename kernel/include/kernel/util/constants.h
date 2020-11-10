#ifndef INCLUDE_KERNEL_UTIL_CONSTANTS_H_
#define INCLUDE_KERNEL_UTIL_CONSTANTS_H_

#define NUM_CORES                      4

// peripherals base address
#define PBASE                          0x3F000000

// GPIO register addresses
#define GPFSEL1                        (PBASE+0x00200004)
#define GPSET0                         (PBASE+0x0020001C)
#define GPCLR0                         (PBASE+0x00200028)
#define GPPUD                          (PBASE+0x00200094)
#define GPPUDCLK0                      (PBASE+0x00200098)

// UART register addresses
#define AUX_ENABLES                    (PBASE+0x00215004)
#define AUX_MU_IO_REG                  (PBASE+0x00215040)
#define AUX_MU_IER_REG                 (PBASE+0x00215044)
#define AUX_MU_IIR_REG                 (PBASE+0x00215048)
#define AUX_MU_LCR_REG                 (PBASE+0x0021504C)
#define AUX_MU_MCR_REG                 (PBASE+0x00215050)
#define AUX_MU_LSR_REG                 (PBASE+0x00215054)
#define AUX_MU_MSR_REG                 (PBASE+0x00215058)
#define AUX_MU_SCRATCH                 (PBASE+0x0021505C)
#define AUX_MU_CNTL_REG                (PBASE+0x00215060)
#define AUX_MU_STAT_REG                (PBASE+0x00215064)
#define AUX_MU_BAUD_REG                (PBASE+0x00215068)

// SCTLR_EL1, System Control Register (EL1), Page 2654 of AArch64-Reference-Manual.
#define SCTLR_RESERVED                 (3 << 28) | (3 << 22) | (1 << 20) | (1 << 11)
#define SCTLR_EE_LITTLE_ENDIAN         (0 << 25)
#define SCTLR_EOE_LITTLE_ENDIAN        (0 << 24)
#define SCTLR_I_CACHE_DISABLED         (0 << 12)
#define SCTLR_D_CACHE_DISABLED         (0 << 2)
#define SCTLR_MMU_DISABLED             (0 << 0)
#define SCTLR_MMU_ENABLED              (1 << 0)
#define SCTLR_VALUE_MMU_DISABLED       (SCTLR_RESERVED | SCTLR_EE_LITTLE_ENDIAN | SCTLR_I_CACHE_DISABLED | SCTLR_D_CACHE_DISABLED | SCTLR_MMU_DISABLED)

// HCR_EL2, Hypervisor Configuration Register (EL2), Page 2487 of AArch64-Reference-Manual.
#define HCR_RW                         (1 << 31)
#define HCR_VALUE                      HCR_RW

// SCR_EL3, Secure Configuration Register (EL3), Page 2648 of AArch64-Reference-Manual.
#define SCR_RESERVED                   (3 << 4)
#define SCR_RW                         (1 << 10)
#define SCR_NS                         (1 << 0)
#define SCR_VALUE                      (SCR_RESERVED | SCR_RW | SCR_NS)

// SPSR_EL3, Saved Program Status Register (EL3) Page 389 of AArch64-Reference-Manual.
#define SPSR_MASK_ALL                  (7 << 6)
#define SPSR_EL1h                      (5 << 0)
#define SPSR_VALUE                     (SPSR_MASK_ALL | SPSR_EL1h)

// FPEXC
#define FPEXC_ENABLE                   (1 << 30)
#define FPEXC_VALUE                    (FPEXC_ENABLE)

#endif  // INCLUDE_KERNEL_UTIL_CONSTANTS_H_
