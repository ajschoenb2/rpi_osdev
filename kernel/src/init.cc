#include "kernel/init.h"

#include "kernel/machine.h"
#include "kernel/uart.h"

struct Stack {
    static constexpr int BYTES = 4096;
    uint64_t bytes[BYTES] __attribute__ ((aligned(16)));
};  // struct Stack

Stack stacks[4];  // one per CPU

extern "C" uintptr_t pickKernelStack(void) {
    return reinterpret_cast<uintptr_t>(&stacks[getCpu()].bytes[Stack::BYTES]);
}

extern "C" void kernelInit(void) {
    Uart uart{};
    const char* msg = "hello, world\n";
    int len = 13;
    for (int i = 0; i < len; i++) {
        uart.put(msg[i]);
    }
    shutdown();
}
