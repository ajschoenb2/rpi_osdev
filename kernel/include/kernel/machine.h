#ifndef INCLUDE_KERNEL_MACHINE_H_
#define INCLUDE_KERNEL_MACHINE_H_

#include "kernel/stdint.h"

extern "C" void entry(void);

extern "C" void* memcpy(void* dest, const void* src, size_t n);
extern "C" void* bzero(void* dest, size_t n);

extern "C" void put32(uint32_t dest, uint32_t val);
extern "C" uint32_t get32(uint32_t src);
extern "C" void delay(uint32_t cycles);
extern "C" uint64_t getCpu();

extern "C" void shutdown();

#endif  // INCLUDE_KERNEL_MACHINE_H_
