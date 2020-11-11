#include "kernel/util/heap.h"

#include "kernel/sync/atomic.h"
// #include "kernel/sync/blocking_lock.h"
#include "kernel/util/debug.h"
#include "kernel/util/stdint.h"

/* A first-fit heap */

typedef SpinLock HeapLockType;

namespace heap {
    
static size_t *array;
static size_t len;
static size_t safe = 0;
static size_t avail = 0;
static HeapLockType lock;

void makeTaken(size_t i, size_t ints);
void makeAvail(size_t i, size_t ints);

size_t abs(size_t x) {
    if (x < 0) return -x; else return x;
}

size_t size(size_t i) {
    return abs(array[i]);
}

size_t headerFromFooter(size_t i) {
    return i - size(i) + 1;
}

size_t footerFromHeader(size_t i) {
    return i + size(i) - 1;
}
    
size_t sanity(size_t i) {
    if (safe) {
        if (i == 0) return 0;
        if ((i < 0) || (i >= len)) {
            Debug::panic("bad header index %d\n", i);
            return i;
        }
        size_t footer = footerFromHeader(i);
        if ((footer < 0) || (footer >= len)) {
            Debug::panic("bad footer index %d\n", footer);
            return i;
        }
        size_t hv = array[i];
        size_t fv = array[footer];
  
        if (hv != fv) {
            Debug::panic("bad block at %d, hv:%d fv:%d\n", i, hv, fv);
            return i;
        }
    }

    return i;
}

size_t left(size_t i) {
    return sanity(headerFromFooter(i - 1));
}

size_t right(size_t i) {
    return sanity(i + size(i));
}

size_t next(size_t i) {
    return sanity(array[i + 1]);
}

size_t prev(size_t i) {
    return sanity(array[i + 2]);
}

void setNext(size_t i, size_t x) {
    array[i + 1] = x;
}

void setPrev(size_t i, size_t x) {
    array[i + 2] = x;
}

void remove(size_t i) {
    size_t prevIndex = prev(i);
    size_t nextIndex = next(i);

    if (prevIndex == 0) {
        /* at head */
        avail = nextIndex;
    } else {
        /* in the middle */
        setNext(prevIndex, nextIndex);
    }
    if (nextIndex != 0) {
        setPrev(nextIndex, prevIndex);
    }
}

void makeAvail(size_t i, size_t ints) {
    array[i] = ints;
    array[footerFromHeader(i)] = ints;    
    setNext(i, avail);
    setPrev(i, 0);
    if (avail != 0) {
        setPrev(avail, i);
    }
    avail = i;
}

void makeTaken(size_t i, size_t ints) {
    array[i] = -ints;
    array[footerFromHeader(i)] = -ints;    
}

size_t isAvail(size_t i) {
    return array[i] > 0;
}

size_t isTaken(size_t i) {
    return array[i] < 0;
}
};

void heapInit(void* base, size_t bytes) {
    /* can't say new becasue we're initializing the heap */
    heap::array = reinterpret_cast<size_t*>(base);
    heap::len = bytes / 4;
    heap::makeTaken(0, 2);
    heap::makeAvail(2, heap::len-4);
    heap::makeTaken(heap::len - 2, 2);
}

void* malloc(size_t bytes) {
    //Debug::printf("malloc(%d)\n",bytes);
    if (bytes == 0) return reinterpret_cast<void*>(heap::array);

    size_t ints = ((bytes + 3) / 4) + 2;
    if (ints < 4) ints = 4;

    LockGuard<HeapLockType> g{heap::lock};

    void* res = 0;

    size_t mx = 0x7FFFFFFFFFFFFFFF;
    size_t it = 0;
    size_t countDown = 20;
    size_t p = heap::avail;
    while (p != 0) {
        if (!heap::isAvail(p)) {
            Debug::panic("block is not available in malloc %p\n", p);
        }
        size_t sz = heap::size(p);

        if (sz >= ints) {
            if (sz < mx) {
                mx = sz;
                it = p;
            }
            countDown--;
            if (countDown == 0) break;
        }
        p = heap::next(p);
    }

    if (it != 0) {
        heap::remove(it);
        size_t extra = mx - ints;
        if (extra >= 4) {
            heap::makeTaken(it, ints);
            heap::makeAvail(it + ints, extra);
        } else {
            heap::makeTaken(it, mx);
        }
        res = &heap::array[it + 1];
    }

    return res;
}

void free(void* p) {
    if (p == 0) return;
    if (p == reinterpret_cast<void*>(heap::array)) return;

    LockGuard<HeapLockType> g{heap::lock};

    size_t idx = (((reinterpret_cast<uintptr_t>(p)) - (reinterpret_cast<uintptr_t>(heap::array))) / 4) - 1;
    heap::sanity(idx);
    if (!heap::isTaken(idx)) {
        Debug::panic("freeing free block, p:%x idx:%ld\n", reinterpret_cast<uintptr_t>(p), idx);
        return;
    }

    size_t sz = heap::size(idx);

    size_t leftIndex = heap::left(idx);
    size_t rightIndex = heap::right(idx);

    if (heap::isAvail(leftIndex)) {
        heap::remove(leftIndex);
        idx = leftIndex;
        sz += heap::size(leftIndex);
    }

    if (heap::isAvail(rightIndex)) {
        heap::remove(rightIndex);
        sz += heap::size(rightIndex);
    }

    heap::makeAvail(idx, sz);
}


/*****************/
/* C++ operators */
/*****************/

void* operator new(uint32_t size) {
    void* p =  malloc(size);
    if (p == 0) Debug::panic("out of memory");
    return p;
}

void operator delete(void* p) noexcept {
    return free(p);
}

void operator delete(void* p, uint32_t sz) {
    return free(p);
}

void* operator new[](uint32_t size) {
    void* p =  malloc(size);
    if (p == 0) Debug::panic("out of memory");
    return p;
}

void operator delete[](void* p) noexcept {
    return free(p);
}

void operator delete[](void* p, uint32_t sz) {
    return free(p);
}
