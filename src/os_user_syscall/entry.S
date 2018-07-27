// See LICENSE for license details.

#include "encoding.h"

#define RISCV_PGSHIFT 12
#define RISCV_PGSIZE (1 << RISCV_PGSHIFT)
#define REGBYTES 4
#define STORE sw
#define LOAD lw

    .section .text.init,"ax",@progbits
    .align 2
    .globl _start
_start:

    #csrw mscratch, x0
    
    #la t0, __alltraps
    #csrw mtvec, t0

    la sp, stacks + RISCV_PGSIZE

    j kern_entry


    .macro SAVE_ALL

    # If coming from userspace, preserve the user stack pointer and load
    # the kernel stack pointer. If we came from the kernel, sscratch
    # will contain 0, and we should continue on the current stack.
    csrrw sp, mscratch, sp
    bnez sp, _save_context

_restore_kernel_sp:
    csrr sp, mscratch
_save_context:
    addi sp, sp, -36 * REGBYTES
    # save x registers
    STORE x0, 0*REGBYTES(sp)
    STORE x1, 1*REGBYTES(sp)
    STORE x3, 3*REGBYTES(sp)
    STORE x4, 4*REGBYTES(sp)
    STORE x5, 5*REGBYTES(sp)
    STORE x6, 6*REGBYTES(sp)
    STORE x7, 7*REGBYTES(sp)
    STORE x8, 8*REGBYTES(sp)
    STORE x9, 9*REGBYTES(sp)
    STORE x10, 10*REGBYTES(sp)
    STORE x11, 11*REGBYTES(sp)
    STORE x12, 12*REGBYTES(sp)
    STORE x13, 13*REGBYTES(sp)
    STORE x14, 14*REGBYTES(sp)
    STORE x15, 15*REGBYTES(sp)
    STORE x16, 16*REGBYTES(sp)
    STORE x17, 17*REGBYTES(sp)
    STORE x18, 18*REGBYTES(sp)
    STORE x19, 19*REGBYTES(sp)
    STORE x20, 20*REGBYTES(sp)
    STORE x21, 21*REGBYTES(sp)
    STORE x22, 22*REGBYTES(sp)
    STORE x23, 23*REGBYTES(sp)
    STORE x24, 24*REGBYTES(sp)
    STORE x25, 25*REGBYTES(sp)
    STORE x26, 26*REGBYTES(sp)
    STORE x27, 27*REGBYTES(sp)
    STORE x28, 28*REGBYTES(sp)
    STORE x29, 29*REGBYTES(sp)
    STORE x30, 30*REGBYTES(sp)
    STORE x31, 31*REGBYTES(sp)

    # get sr, epc, badvaddr, cause
    # Set sscratch register to 0, so that if a recursive exception
    # occurs, the exception vector knows it came from the kernel
    csrrw s0, mscratch, x0
    csrr s1, mstatus
    csrr s2, mepc
    csrr s3, mbadaddr
    csrr s4, mcause

    STORE s0, 2*REGBYTES(sp)
    STORE s1, 32*REGBYTES(sp)
    STORE s2, 33*REGBYTES(sp)
    STORE s3, 34*REGBYTES(sp)
    STORE s4, 35*REGBYTES(sp)
    .endm

    .macro RESTORE_ALL

    LOAD s1, 32*REGBYTES(sp)
    LOAD s2, 33*REGBYTES(sp)
    li t0, MSTATUS_MPP

    and s0, s1, t0
    #andi s0, s1, SSTATUS_SPP
    bnez s0, _restore_context  #this means it traps mstatusfrom U mode

_save_kernel_sp:
    # Save unwound kernel stack pointer in sscratch
    addi s0, sp, 36 * REGBYTES
    csrw mscratch, s0
_restore_context:
    csrw mstatus, s1
    csrw mepc, s2

    # restore x registers
    LOAD x1, 1*REGBYTES(sp)
    LOAD x3, 3*REGBYTES(sp)
    LOAD x4, 4*REGBYTES(sp)
    LOAD x5, 5*REGBYTES(sp)
    LOAD x6, 6*REGBYTES(sp)
    LOAD x7, 7*REGBYTES(sp)
    LOAD x8, 8*REGBYTES(sp)
    LOAD x9, 9*REGBYTES(sp)
    LOAD x10, 10*REGBYTES(sp)
    LOAD x11, 11*REGBYTES(sp)
    LOAD x12, 12*REGBYTES(sp)
    LOAD x13, 13*REGBYTES(sp)
    LOAD x14, 14*REGBYTES(sp)
    LOAD x15, 15*REGBYTES(sp)
    LOAD x16, 16*REGBYTES(sp)
    LOAD x17, 17*REGBYTES(sp)
    LOAD x18, 18*REGBYTES(sp)
    LOAD x19, 19*REGBYTES(sp)
    LOAD x20, 20*REGBYTES(sp)
    LOAD x21, 21*REGBYTES(sp)
    LOAD x22, 22*REGBYTES(sp)
    LOAD x23, 23*REGBYTES(sp)
    LOAD x24, 24*REGBYTES(sp)
    LOAD x25, 25*REGBYTES(sp)
    LOAD x26, 26*REGBYTES(sp)
    LOAD x27, 27*REGBYTES(sp)
    LOAD x28, 28*REGBYTES(sp)
    LOAD x29, 29*REGBYTES(sp)
    LOAD x30, 30*REGBYTES(sp)
    LOAD x31, 31*REGBYTES(sp)
    # restore sp last
    LOAD x2, 2*REGBYTES(sp)
    .endm

    .globl __alltraps
__alltraps:
    SAVE_ALL

    move  a0, sp
    jal trap
    # sp should be the same as before "jal trap"

    .globl __trapret
__trapret:
    RESTORE_ALL
    # return from supervisor call
    mret
 

# void switch_to(struct context* from, struct context* to)
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
