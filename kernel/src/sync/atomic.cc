#include "kernel/sync/atomic.h"
#include "kernel/util/machine.h"

namespace interrupts {

    bool isDisabled() {
        uint64_t daif = getDAIF();
        return ((daif >> 7) & 0x1) == 1;
    }

    bool disable() {
        bool was = isDisabled();
        if (!was) disableIRQ();
        return was;
    }

    void restore(bool was) {
        if (!was) enableIRQ();
    }

};  // namespace interrupts

// SpinLock
SpinLock::SpinLock() : taken(false) {}

void SpinLock::lock(void) {
    while (taken.exchange(true)) spin();
}

void SpinLock::unlock(void) {
    taken.set(false);
}
// end SpinLock

// InterruptSafeLock
void InterruptSafeLock::lock() {
    while (true) {
        bool was = interrupts::disable();
        if (!taken.exchange(true)) {
            this->was = was;
            return;
        }
        interrupts::restore(was);

        spin();
    }
}

void InterruptSafeLock::unlock() {
    taken.set(false);
    interrupts::restore(was);
}
// end InterruptSafeLock
