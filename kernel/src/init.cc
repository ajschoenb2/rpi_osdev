#include "kernel/init.h"

#include "kernel/io/uart.h"
#include "kernel/util/debug.h"
#include "kernel/util/machine.h"

struct Stack {
    static constexpr int BYTES = 4096;
    uint64_t bytes[BYTES] __attribute__ ((aligned(16)));
};  // struct Stack

Stack stacks[NUM_CORES];  // one per CPU

extern "C" uintptr_t pickKernelStack(void) {
    return reinterpret_cast<uintptr_t>(&stacks[getCpu()].bytes[Stack::BYTES]);
}

extern "C" void kernelInit(void) {
    Uart uart{};
    Debug::init(&uart);
    Debug::printf("****************************************************************\n");
    Debug::printf("*** Hello, world\n");
    Debug::shutdown();
}
