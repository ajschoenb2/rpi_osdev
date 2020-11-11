    .globl contextSwitch
contextSwitch:
saveRegs:
    str lr, [sp, #-16]!

    stp x8, x9, [x0], #16
    stp x10, x11, [x0], #16
    stp x12, x13, [x0], #16
    stp x14, x15, [x0], #16
    stp x16, x17, [x0], #16
    stp x18, x19, [x0], #16
    stp x20, x21, [x0], #16
    stp x22, x23, [x0], #16
    stp x24, x25, [x0], #16
    stp x26, x27, [x0], #16
    stp x28, x29, [x0], #16

    mov x2, sp
    str x2, [x0]
    mov x0, x1

    .globl loadRegs
loadRegs:
    ldp x8, x9, [x0], #16
    ldp x10, x11, [x0], #16
    ldp x12, x13, [x0], #16
    ldp x14, x15, [x0], #16
    ldp x16, x17, [x0], #16
    ldp x18, x19, [x0], #16
    ldp x20, x21, [x0], #16
    ldp x22, x23, [x0], #16
    ldp x24, x25, [x0], #16
    ldp x26, x27, [x0], #16
    ldp x28, x29, [x0], #16

    ldr x2, [x0]
    mov sp, x2

    ldr lr, [sp], #16
    ret
