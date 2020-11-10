#ifndef INCLUDE_KERNEL_SYNC_ATOMIC_H_
#define INCLUDE_KERNEL_SYNC_ATOMIC_H_

template <typename T>
class AtomicPtr {
    volatile T *ptr;

public:
    AtomicPtr() : ptr(nullptr) {}
    AtomicPtr(T *x) : ptr(x) {}
    AtomicPtr<T>& operator= (T v) {
        __atomic_store_n(ptr,v,__ATOMIC_SEQ_CST);
        return *this;
    }
    operator T () const {
        return __atomic_load_n(ptr,__ATOMIC_SEQ_CST);
    }
    T fetchAdd(T inc) {
        return __atomic_fetch_add(ptr,inc,__ATOMIC_SEQ_CST);
    }
    T addFetch(T inc) {
        return __atomic_add_fetch(ptr,inc,__ATOMIC_SEQ_CST);
    }
    void set(T inc) {
        return __atomic_store_n(ptr,inc,__ATOMIC_SEQ_CST);
    }
    T get(void) {
        return __atomic_load_n(ptr,__ATOMIC_SEQ_CST);
    }
    T exchange(T v) {
        T ret;
        __atomic_exchange(ptr,&v,&ret,__ATOMIC_SEQ_CST);
        return ret;
    }
};  // class AtomicPtr

template <typename T>
class Atomic {
    volatile T value;
    
public:
    Atomic(T x) : value(x) {}
    Atomic<T>& operator= (T v) {
        __atomic_store_n(&value,v,__ATOMIC_SEQ_CST);
        return *this;
    }
    operator T () const {
        return __atomic_load_n(&value,__ATOMIC_SEQ_CST);
    }
    T fetchAdd(T inc) {
        return __atomic_fetch_add(&value,inc,__ATOMIC_SEQ_CST);
    }
    T addFetch(T inc) {
        return __atomic_add_fetch(&value,inc,__ATOMIC_SEQ_CST);
    }
    void set(T inc) {
        return __atomic_store_n(&value,inc,__ATOMIC_SEQ_CST);
    }
    T get(void) {
        return __atomic_load_n(&value,__ATOMIC_SEQ_CST);
    }
    T exchange(T v) {
        T ret;
        __atomic_exchange(&value,&v,&ret,__ATOMIC_SEQ_CST);
        return ret;
    }
};  // class Atomic

namespace interrupts {

    bool isDisabled();

    bool disable();

    void restore(bool wasDisabled);

    template <typename Work>
    inline void protect(Work work) {
        auto was = disable();
        work();
        restore(was);
    }

};  // namespace interrupts

template <typename T>
class LockGuard {
    T& it;

public:
    inline LockGuard(T& it): it(it) {
        it.lock();
    }
    inline ~LockGuard() {
        it.unlock();
    }
};  // class LockGuard

class NoLock {
public:
    inline void lock() {}
    inline void unlock() {}
};  // class NoLock

class SpinLock {
    Atomic<bool> taken;

public:
    SpinLock();
    SpinLock(const SpinLock&) = delete;

    void lock(void);
    void unlock(void);
};  // class SpinLock

// Is this correct? 
class InterruptSafeLock {
    Atomic<bool> taken;
    volatile bool was;

public:
    InterruptSafeLock() : taken(false), was(false) {}
    InterruptSafeLock(const InterruptSafeLock&) = delete;

    void lock();
    void unlock();
};  // class InterruptSafeLock

#endif  // INCLUDE_KERNEL_SYNC_ATOMIC_H_
