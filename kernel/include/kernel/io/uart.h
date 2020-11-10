#ifndef INCLUDE_KERNEL_IO_UART_H_
#define INCLUDE_KERNEL_IO_UART_H_

#include "kernel/io/io.h"
#include "kernel/util/constants.h"

class Uart : public OutputStream<char> {
public:
    Uart();

    virtual void put(char ch);
    virtual char get();
};  // class Uart

#endif  // INCLUDE_KERNEL_IO_UART_H_
