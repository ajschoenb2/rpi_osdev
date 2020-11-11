#ifndef INCLUDE_KERNEL_UTIL_QUEUE_H_
#define INCLUDE_KERNEL_UTIL_QUEUE_H_

#include "kernel/sync/atomic.h"

template <typename T, typename LockType>
class Queue {
    T* first = nullptr;
    T* last = nullptr;
    LockType lock;
public:
    Queue() : first(nullptr), last(nullptr), lock() {}
    Queue(const Queue&) = delete;

    void add(T* t) {
        LockGuard g{lock};
        t->next = nullptr;
        if (first == nullptr) {
            first = t;
        } else {
            last->next = t;
        }
        last = t;
    }

    T* remove() {
        LockGuard g{lock};
        if (first == nullptr) {
            return nullptr;
        }
        auto it = first;
        first = it->next;
        if (first == nullptr) {
            last = nullptr;
        }
        return it;
    }
};

#endif  // INCLUDE_KERNEL_UTIL_QUEUE_H_
