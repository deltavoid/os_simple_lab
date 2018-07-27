#include <stdint.h>
#include "htif.h"
#include "encoding.h"
#include "trap.h"


extern void __alltraps(void);
extern void __trapret(void);
extern void switch_to(struct pushregs *from, struct pushregs *to);

uint64_t* mtime_p = (uint64_t*)0x00200bff8;
uint64_t* mtimecmp_p = (uint64_t*)0x02004000;
uint64_t timebase = 0x100000;
uint64_t mtimecmp;
//uint64_t ticks;
uint32_t current;

uint64_t get_mtime()
{
    return *mtime_p;
}

void set_mtimecmp(uint64_t when)
{
    uint32_t* src = (uint32_t*)&when;
    uint32_t* dst = (uint32_t*)mtimecmp_p;
    dst[0] = -1;
    dst[1] = src[1];
    dst[0] = src[0];
}

void memset(void* s, uint8_t v, uint32_t len)
{
    uint8_t* a = (uint8_t*)s;
    for (uint32_t i = 0; i < len; i++)
        a[i] = v;
}

#define EXTRACT_FIELD(val, which) (((val) & (which)) / ((which) & ~((which)-1)))
#define INSERT_FIELD(val, which, fieldval) (((val) & ~(which)) | ((fieldval) * ((which) & ~((which)-1))))



#define USER_STACK_SIZE 1024
#define KERN_STACK_SIZE 1024
uint32_t taskA_user_stack[USER_STACK_SIZE];
uint32_t taskB_user_stack[USER_STACK_SIZE];
uint32_t taskA_kern_stack[KERN_STACK_SIZE];
uint32_t taskB_kern_stack[KERN_STACK_SIZE];
struct pushregs taskA_context;
struct pushregs taskB_context;

void taskA()
{
    //while (1)  putstring("A");
    while (1) ;

}

void taskB()
{
    while (1)  putstring("B");

}







void trap(struct trapframe* tf)
{
    putstring("\n");
    put_uint64(get_mtime());
    put_uint32(tf->status);
    put_uint32(tf->cause);
    put_uint32(current);
    set_mtimecmp(mtimecmp += timebase);
    

    //if  (tf->cause == (0x80000000 | IRQ_M_TIMER))
    if  (0)
    {
        if  (current == 1)
        {   current = 0;
            switch_to(&taskA_context, &taskB_context);
        }
        else
        {   current = 1;
            switch_to(&taskB_context, &taskA_context);
        }
    }

}


void kern_entry()
{
    put_uint64(get_mtime());
    putstring("hello world\n");

    mtimecmp = timebase;
    set_mtimecmp(mtimecmp);

    write_csr(mscratch, 0);
    write_csr(mtvec, &__alltraps);
    write_csr(mie, MIP_MTIP);
    //write_csr(mstatus, MSTATUS_MIE);
    


    /*
    uint32_t sp0 = (uint32_t)(taskB_kern_stack + KERN_STACK_SIZE);

    uint32_t sp1 = sp0 - sizeof(struct trapframe);
    struct trapframe* tf = (struct trapframe*)sp1;
    memset(tf, 0, sizeof(struct trapframe));
    //tf->status = MSTATUS_MIE;  //              1000
    tf->status = 0x00001880;  //  0001 1000 1000 0000
    tf->epc = &taskB;
    tf->gpr.sp = sp0;

    struct pushregs* context = (struct pushregs*)&taskB_context;
    memset(context, 0, sizeof(struct pushregs));
    context->ra = &__trapret;
    context->sp = sp1;
    */

    uintptr_t mstatus = 0;
    mstatus = INSERT_FIELD(mstatus, MSTATUS_MPP, PRV_S);
    mstatus = INSERT_FIELD(mstatus, MSTATUS_MPIE, 1);
    mstatus = INSERT_FIELD(mstatus, MSTATUS_SIE, 1);

    /*
    uint32_t taskB_sp0 = (uint32_t)(taskB_kern_stack + KERN_STACK_SIZE);

    uint32_t taskB_sp1 = taskB_sp0 - sizeof(struct trapframe);
    struct trapframe* taskB_tf = (struct trapframe*)taskB_sp1;
    memset(taskB_tf, 0, sizeof(struct trapframe));
    taskB_tf->status = mstatus;
    taskB_tf->epc = &taskB;
    taskB_tf->gpr.sp = (uint32_t)(taskB_user_stack + USER_STACK_SIZE);

    memset(&taskB_context, 0, sizeof(struct pushregs));
    taskB_context.ra = &__trapret;
    taskB_context.sp = taskB_sp1;
    */


    uint32_t taskA_sp0 = (uint32_t)(taskA_kern_stack + KERN_STACK_SIZE);

    uint32_t taskA_sp1 = taskA_sp0 - sizeof(struct trapframe);
    struct trapframe* taskA_tf = (struct trapframe*)taskA_sp1;
    memset(taskA_tf, 0, sizeof(struct trapframe));
    taskA_tf->status = mstatus;
    taskA_tf->epc = &taskA;
    taskA_tf->gpr.sp = (uint32_t)(taskA_user_stack + USER_STACK_SIZE);


    uintptr_t sp = taskA_sp1;

    asm volatile("mv sp, %0 \n\t"
                 "j __trapret \n\t"
                 :
                 : "r"(sp));

    /*    asm volatile("la t0, 1f\n\t"
                 "csrrw t0, mtvec, t0\n\t"
                 "csrw pmpaddr0, %1\n\t"
                 "csrw pmpcfg0, %0\n\t"
                 ".align 2\n\t"
                 "1: csrw mtvec, t0"
                 :
                 : "r"(pmpc), "r"(-1UL)
                 : "t0");*/



    

    //while (1);
    /*current = 1;
    taskA();*/

    

    htif_poweroff();
}
