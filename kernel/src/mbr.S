    #include "kernel/constants.h"
    .section ".text.boot"

    .globl start
start:
    mrs x0, mpidr_el1
    and x0, x0, #0xFF
    cbz x0, master
    b loop

master:
    adr x0, bss_begin
    adr x1, bss_end
    sub x1, x1, x0
    bl bzero

    .global entry
entry:
    .extern pickKernelStack
    .extern kernelInit
    mov sp, #0x100000
    bl pickKernelStack
    mov sp, x0
    bl kernelInit
    b loop

loop:
    b loop
