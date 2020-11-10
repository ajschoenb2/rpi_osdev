#ifndef INCLUDE_KERNEL_UART_H_
#define INCLUDE_KERNEL_UART_H_

#include "kernel/constants.h"
#include "kernel/io.h"

class Uart : public OutputStream<char> {
public:
    Uart();

    virtual void put(char ch);
    virtual char get();
};  // class Uart

#endif  // INCLUDE_KERNEL_UART_H_
