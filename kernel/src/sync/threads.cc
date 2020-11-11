#include "kernel/sync/threads.h"

#include "kernel/util/constants.h"
#include "kernel/util/debug.h"
#include "kernel/util/heap.h"
#include "kernel/util/machine.h"
#include "kernel/sync/atomic.h"

Queue<TCB, InterruptSafeLock> ready_q{};
Queue<TCB, InterruptSafeLock> stopped_q{};
TCB** active_tcb;
TCB** prev_tcb;
TCB** stopped_tcb;

SaveArea::SaveArea() : x8(0), x9(0), x10(0), x11(0), x12(0), x13(0), x14(0), x15(0), x16(0), x17(0), x18(0), x19(0), x20(0), x21(0), x22(0), x23(0), x24(0), x25(0), x26(0), x27(0), x28(0), x29(0), sp(0) {}

TCB::TCB() : next(nullptr), blocked_in(nullptr), held_lock(nullptr), blocked(false), save_area() {}

void threadEntry() {
    active_tcb[getCPU()]->run();
    stop();
}

void handlePrev() {
    if (stopped_tcb[getCPU()] != nullptr) {
        stopped_q.add(stopped_tcb[getCPU()]);
        stopped_tcb[getCPU()] = nullptr;
    } else if (prev_tcb[getCPU()] != nullptr) {
        TCB* prev = prev_tcb[getCPU()];
        prev_tcb[getCPU()] = nullptr;
        if (prev->blocked) {
            prev->blocked_in->add(prev);
            prev->held_lock->unlock();
            prev->held_lock = nullptr;
        } else {
            ready_q.add(prev);
        }
    }
}

void block(Queue<TCB, InterruptSafeLock>* blocked_q, InterruptSafeLock* lock) {
    interrupts::protect([blocked_q, lock] {
        active_tcb[getCPU()]->blocked = true;
        active_tcb[getCPU()]->blocked_in = blocked_q;
        active_tcb[getCPU()]->held_lock = lock;
    });
}

void unblockMe() {
    interrupts::protect([] {
        active_tcb[getCPU()]->blocked = false;
        active_tcb[getCPU()]->blocked_in = nullptr;
        active_tcb[getCPU()]->held_lock = nullptr;
    });
}

void unblock(TCB* tcb) {
    interrupts::protect([tcb] {
        tcb->blocked = false;
        tcb->blocked_in = nullptr;
        tcb->held_lock = nullptr;
        ready_q.add(tcb);
    });
}

void deleteStopped() {
    TCB* stopped = stopped_q.remove();
    while (stopped != nullptr) {
        delete stopped;
        stopped = stopped_q.remove();
    }
}

void threadsInit() {
    ready_q = Queue<TCB, InterruptSafeLock>();
    stopped_q = Queue<TCB, InterruptSafeLock>();
    active_tcb = new TCB*[NUM_CORES];
    prev_tcb = new TCB*[NUM_CORES];
    stopped_tcb = new TCB*[NUM_CORES];
    for (size_t i = 0; i < NUM_CORES; i++) {
        active_tcb[i] = nullptr;
        prev_tcb[i] = nullptr;
        stopped_tcb[i] = nullptr;
    }

    thread([] {
        while (true) {
            deleteStopped();
            spin();
        }
    });
}

void yield() {
    bool was = interrupts::disable();
    TCB* active = active_tcb[getCPU()];
    TCB* next = ready_q.remove();
    if (next == nullptr) {
        interrupts::restore(was);
        if (active->held_lock != nullptr) {
            active->held_lock->unlock();
        }
        return;
    }

    prev_tcb[getCPU()] = active;
    active_tcb[getCPU()] = next;
    contextSwitch(&active->save_area, &next->save_area);
    handlePrev();
    interrupts::restore(was);
}

void stop() {
    interrupts::disable();

    deleteStopped();

    TCB* next = ready_q.remove();
    while (next == nullptr) {
        next = ready_q.remove();
    }

    stopped_tcb[getCPU()] = active_tcb[getCPU()];
    active_tcb[getCPU()] = next;
    loadRegs(&next->save_area);

    Debug::panic("something returned to stop()");
}
