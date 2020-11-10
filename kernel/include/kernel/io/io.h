#ifndef INCLUDE_KERNEL_IO_IO_H_
#define INCLUDE_KERNEL_IO_IO_H_

template<typename T> class OutputStream {
public:
    virtual void put(T v) = 0;
};  // class OutputStream

template<typename T> class InputStream {
public:
    virtual T get() = 0;
};  // class InputStream

#endif  // INCLUDE_KERNEL_IO_IO_H_
