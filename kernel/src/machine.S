    .globl bzero
bzero:
    str xzr, [x0], #8
    subs x1, x1, #8
    b.gt bzero
    ret

    .globl put32
put32:
    str w1, [x0]
    ret

    .globl get32
get32:
    ldr w0, [x0]
    ret

    .globl delay
delay:
    subs x0, x0, #1
    bne delay
    ret

    .globl getCpu
getCpu:
    mrs x0, mpidr_el1
    and x0, x0, #0xff
    ret

    .globl shutdown
shutdown:
    mov x1, #0x26
    movk x1, #2, lsl #16
    str x1, [sp, #0]
    mov x0, #0
    str x0, [sp, #8]
    mov x1, sp
    mov w0, #0x18
shutdown_loop:
    hlt #0xf000
    b shutdown_loop
