#ifndef INCLUDE_KERNEL_SYNC_THREADS_H_
#define INCLUDE_KERNEL_SYNC_THREADS_H_

#include "kernel/util/machine.h"
#include "kernel/util/heap.h"
#include "kernel/util/queue.h"
#include "kernel/util/stdint.h"
#include "kernel/sync/atomic.h"

constexpr size_t STACK_BYTES = 8 * 1024;

struct SaveArea {
    uint64_t x8;
    uint64_t x9;
    uint64_t x10;
    uint64_t x11;
    uint64_t x12;
    uint64_t x13;
    uint64_t x14;
    uint64_t x15;
    uint64_t x16;
    uint64_t x17;
    uint64_t x18;
    uint64_t x19;
    uint64_t x20;
    uint64_t x21;
    uint64_t x22;
    uint64_t x23;
    uint64_t x24;
    uint64_t x25;
    uint64_t x26;
    uint64_t x27;
    uint64_t x28;
    uint64_t x29;
    uint64_t sp;

    SaveArea();
};  // struct SaveArea

class TCB {
public:
    TCB* next;

    Queue<TCB, InterruptSafeLock>* blocked_in;
    InterruptSafeLock* held_lock;
    bool blocked;

    SaveArea save_area;

    TCB();
    virtual ~TCB() {}

    virtual void init() = 0;
    virtual void run() = 0;
};  // class TCB

extern Queue<TCB, InterruptSafeLock> ready_q;
extern Queue<TCB, InterruptSafeLock> stopped_q;

extern TCB** active_tcb;
extern TCB** prev_tcb;
extern TCB** stopped_tcb;

extern void threadEntry();
extern void handlePrev();
extern void block(Queue<TCB, InterruptSafeLock>* blocked_q, InterruptSafeLock* lock);
extern void unblockMe();
extern void unblock(TCB* tcb);
extern void deleteStopped();

template <typename T>
class TCBWithStack : public TCB {
private:
    T work;
    uint64_t stack[STACK_BYTES / sizeof(uint64_t)];

public:
    TCBWithStack(T work) : work(work) {
        init();
    }

    ~TCBWithStack() {}

    void init() override {
        uint64_t stack_elems = STACK_BYTES / sizeof(uint64_t);
        uint64_t stack_max = stack_elems - 1;
        save_area.sp = reinterpret_cast<uintptr_t>(&stack[stack_max]);
        stack[stack_max] = reinterpret_cast<uintptr_t>(threadEntry);
    }

    void run() override {
        handlePrev();
        enableIRQ();
        work();
    }
};  // class TCBImpl

extern void threadsInit();
extern void yield();
extern void stop();

extern "C" void loadRegs(SaveArea*);
extern "C" void contextSwitch(SaveArea*, SaveArea*);

template <typename T>
void thread(T work) {
    TCBWithStack<T>* tcb = new TCBWithStack<T>(work);
    ready_q.add(tcb);
}

#endif  // INCLUDE_KERNEL_SYNC_THREADS_H_
