    #include "kernel/util/constants.h"
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
    # switch to EL1
    ldr x0, =SCTLR_VALUE_MMU_DISABLED
    msr sctlr_el1, x0

    ldr x0, =HCR_VALUE
    msr hcr_el2, x0

    # ldr x0, =SCR_VALUE
    # msr scr_el2, x0

    ldr x0, =FPEXC_VALUE
    msr fpexc32_el2, x0

    ldr x0, =SPSR_VALUE
    msr spsr_el2, x0
    
    adr x0, el1_entry
    msr elr_el2, x0

    eret

el1_entry:
    .extern pickKernelStack
    .extern kernelInit
    mov sp, #0x100000
    bl pickKernelStack
    mov sp, x0
    bl kernelInit
    b loop

loop:
    b loop
