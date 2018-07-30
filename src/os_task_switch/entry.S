// See LICENSE for license details.

#define RISCV_PGSHIFT 12
#define RISCV_PGSIZE (1 << RISCV_PGSHIFT)
#define REGBYTES 4

    .section .text.init,"ax",@progbits
    .align 2
    .globl _start
_start:

    la sp, stacks + RISCV_PGSIZE

    j kern_entry


    .macro SAVE_ALL  //这里与os_time_interrupt相比增加了两个寄存器mbadaddr(1.10叫mtval)和mcause。mcause存储中断发生的原因，mtval存储于中断相关的值。在本例中mtval并未使用。

    csrw mscratch, sp
    addi sp, sp, -36 * REGBYTES
    sw x0, 0*REGBYTES(sp)
    sw x1, 1*REGBYTES(sp)
    sw x3, 3*REGBYTES(sp)
    sw x4, 4*REGBYTES(sp)
    sw x5, 5*REGBYTES(sp)
    sw x6, 6*REGBYTES(sp)
    sw x7, 7*REGBYTES(sp)
    sw x8, 8*REGBYTES(sp)
    sw x9, 9*REGBYTES(sp)
    sw x10, 10*REGBYTES(sp)
    sw x11, 11*REGBYTES(sp)
    sw x12, 12*REGBYTES(sp)
    sw x13, 13*REGBYTES(sp)
    sw x14, 14*REGBYTES(sp)
    sw x15, 15*REGBYTES(sp)
    sw x16, 16*REGBYTES(sp)
    sw x17, 17*REGBYTES(sp)
    sw x18, 18*REGBYTES(sp)
    sw x19, 19*REGBYTES(sp)
    sw x20, 20*REGBYTES(sp)
    sw x21, 21*REGBYTES(sp)
    sw x22, 22*REGBYTES(sp)
    sw x23, 23*REGBYTES(sp)
    sw x24, 24*REGBYTES(sp)
    sw x25, 25*REGBYTES(sp)
    sw x26, 26*REGBYTES(sp)
    sw x27, 27*REGBYTES(sp)
    sw x28, 28*REGBYTES(sp)
    sw x29, 29*REGBYTES(sp)
    sw x30, 30*REGBYTES(sp)
    sw x31, 31*REGBYTES(sp)

    csrr s0, mscratch
    csrr s1, mstatus
    csrr s2, mepc
    csrr s3, mbadaddr
    csrr s4, mcause
    
    sw s0, 2*REGBYTES(sp)
    sw s1, 32*REGBYTES(sp)
    sw s2, 33*REGBYTES(sp)
    sw s3, 34*REGBYTES(sp)
    sw s4, 35*REGBYTES(sp)
    .endm

    .macro RESTORE_ALL

    # Restore all of the registers.
    lw s1, 32*REGBYTES(sp)
    lw s2, 33*REGBYTES(sp)

    csrw mstatus, s1
    csrw mepc, s2
    
    lw x1, 1*REGBYTES(sp)
    lw x3, 3*REGBYTES(sp)
    lw x4, 4*REGBYTES(sp)
    lw x5, 5*REGBYTES(sp)
    lw x6, 6*REGBYTES(sp)
    lw x7, 7*REGBYTES(sp)
    lw x8, 8*REGBYTES(sp)
    lw x9, 9*REGBYTES(sp)
    lw x10,10*REGBYTES(sp)
    lw x11,11*REGBYTES(sp)
    lw x12,12*REGBYTES(sp)
    lw x13,13*REGBYTES(sp)
    lw x14,14*REGBYTES(sp)
    lw x15,15*REGBYTES(sp)
    lw x16,16*REGBYTES(sp)
    lw x17,17*REGBYTES(sp)
    lw x18,18*REGBYTES(sp)
    lw x19,19*REGBYTES(sp)
    lw x20,20*REGBYTES(sp)
    lw x21,21*REGBYTES(sp)
    lw x22,22*REGBYTES(sp)
    lw x23,23*REGBYTES(sp)
    lw x24,24*REGBYTES(sp)
    lw x25,25*REGBYTES(sp)
    lw x26,26*REGBYTES(sp)
    lw x27,27*REGBYTES(sp)
    lw x28,28*REGBYTES(sp)
    lw x29,29*REGBYTES(sp)
    lw x30,30*REGBYTES(sp)
    lw x31,31*REGBYTES(sp)
    # restore sp last
    lw x2, 2*REGBYTES(sp)
    .endm

    .globl __alltraps
__alltraps:
    SAVE_ALL

    move a0, sp
    jal trap

    .globl __trapret
__trapret:
    RESTORE_ALL
    mret


# void switch_to(struct context* from, struct context* to)  //在任务切换时保存旧任务的上下文并恢复新任务的上下文。
    .globl switch_to
switch_to:
    # save from's registers
    sw x0, 0*REGBYTES(a0)
    sw x1, 1*REGBYTES(a0)
    sw x2, 2*REGBYTES(a0)
    sw x3, 3*REGBYTES(a0)
    sw x4, 4*REGBYTES(a0)
    sw x5, 5*REGBYTES(a0)
    sw x6, 6*REGBYTES(a0)
    sw x7, 7*REGBYTES(a0)
    sw x8, 8*REGBYTES(a0)
    sw x9, 9*REGBYTES(a0)
    sw x10, 10*REGBYTES(a0)
    sw x11, 11*REGBYTES(a0)
    sw x12, 12*REGBYTES(a0)
    sw x13, 13*REGBYTES(a0)
    sw x14, 14*REGBYTES(a0)
    sw x15, 15*REGBYTES(a0)
    sw x16, 16*REGBYTES(a0)
    sw x17, 17*REGBYTES(a0)
    sw x18, 18*REGBYTES(a0)
    sw x19, 19*REGBYTES(a0)
    sw x20, 20*REGBYTES(a0)
    sw x21, 21*REGBYTES(a0)
    sw x22, 22*REGBYTES(a0)
    sw x23, 23*REGBYTES(a0)
    sw x24, 24*REGBYTES(a0)
    sw x25, 25*REGBYTES(a0)
    sw x26, 26*REGBYTES(a0)
    sw x27, 27*REGBYTES(a0)
    sw x28, 28*REGBYTES(a0)
    sw x29, 29*REGBYTES(a0)
    sw x30, 30*REGBYTES(a0)
    sw x31, 31*REGBYTES(a0)

    # restore to's registers
    lw x1, 1*REGBYTES(a1)
    lw x2, 2*REGBYTES(a1)
    lw x3, 3*REGBYTES(a1)
    lw x4, 4*REGBYTES(a1)
    lw x5, 5*REGBYTES(a1)
    lw x6, 6*REGBYTES(a1)
    lw x7, 7*REGBYTES(a1)
    lw x8, 8*REGBYTES(a1)
    lw x9, 9*REGBYTES(a1)
    lw x10,10*REGBYTES(a1)
    lw x11,11*REGBYTES(a1)
    lw x12,12*REGBYTES(a1)
    lw x13,13*REGBYTES(a1)
    lw x14,14*REGBYTES(a1)
    lw x15,15*REGBYTES(a1)
    lw x16,16*REGBYTES(a1)
    lw x17,17*REGBYTES(a1)
    lw x18,18*REGBYTES(a1)
    lw x19,19*REGBYTES(a1)
    lw x20,20*REGBYTES(a1)
    lw x21,21*REGBYTES(a1)
    lw x22,22*REGBYTES(a1)
    lw x23,23*REGBYTES(a1)
    lw x24,24*REGBYTES(a1)
    lw x25,25*REGBYTES(a1)
    lw x26,26*REGBYTES(a1)
    lw x27,27*REGBYTES(a1)
    lw x28,28*REGBYTES(a1)
    lw x29,29*REGBYTES(a1)
    lw x30,30*REGBYTES(a1)
    lw x31,31*REGBYTES(a1)

    ret



  .bss
  .align RISCV_PGSHIFT
stacks:
  .skip RISCV_PGSIZE
