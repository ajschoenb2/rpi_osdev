#ifndef INCLUDE_KERNEL_INIT_H_
#define INCLUDE_KERNEL_INIT_H_

#include "kernel/util/stdint.h"

extern "C" uintptr_t pickKernelStack(void);
extern "C" void kernelInit(void);

#endif  // INCLUDE_KERNEL_INIT_H_
