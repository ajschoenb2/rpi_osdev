#include "kernel/init.h"

#include "kernel/io/uart.h"
#include "kernel/kernel.h"
#include "kernel/sync/atomic.h"
#include "kernel/util/debug.h"
#include "kernel/util/machine.h"

struct Stack {
    static constexpr int BYTES = 4096;
    uint64_t bytes[BYTES] __attribute__ ((aligned(16)));
};  // struct Stack

static Stack stacks[NUM_CORES];  // one per CPU

extern "C" uintptr_t pickKernelStack(void) {
    return reinterpret_cast<uintptr_t>(&stacks[getCPU()].bytes[Stack::BYTES]);
}

static Atomic<uint64_t> started{0};
static Atomic<uint64_t> here{0};
static Atomic<bool> init_done{false};

extern "C" void kernelInit(void) {
    uint64_t me = started.fetchAdd(1);

    if (me == 0) {
        Uart uart{};
        Debug::init(&uart);
        Debug::printf("----------------------------------------------------------------\n");
        Debug::printf("| hello, world\n");

        Debug::printf("| waking up other cores...\n");
        for (uint64_t id = 0; id < NUM_CORES; id++) {
            if (id != getCPU()) {
                Debug::printf("| initialize %ld\n", id);
                Debug::printf("| reset %ld\n", id);
                Debug::printf("|    entry:0x%x\n", entry);
                wakeupCPU(id, reinterpret_cast<uint64_t>(entry));
            }
        }

        init_done.set(true);
    }

    while (!init_done.get()) {}

    uint64_t order = here.addFetch(1);
    if (order == NUM_CORES) {
        kernelMain();
        Debug::shutdown();
    }
    while (true) spin();
}
