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
uint32_t current; //标记当前任务，1为taskA，0为taskB

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

void memset(void* s, uint8_t v, uint32_t len)  //清空一段内存
{
    uint8_t* a = (uint8_t*)s;
    for (uint32_t i = 0; i < len; i++)
        a[i] = v;
}



#define USER_STACK_SIZE 1024
#define KERN_STACK_SIZE 1024
//uint32_t taskA_kern_stack[KERN_STACK_SIZE]; //taskA使用原有的系统栈，并未使用自己的内核栈
uint32_t taskB_kern_stack[KERN_STACK_SIZE];
struct pushregs taskA_context;  //上下文存储一个任务执行时所有通用寄存器的值
struct pushregs taskB_context;

void taskA()
{
    while (1)  putstring("A");

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
    

    if  (tf->cause == (0x80000000 | IRQ_M_TIMER))  // 中断的最高位置1，表明这是一个中断
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

    write_csr(mtvec, &__alltraps);
    write_csr(mie, MIP_MTIP);
    write_csr(mstatus, MSTATUS_MIE);


    uint32_t sp0 = (uint32_t)(taskB_kern_stack + KERN_STACK_SIZE);  //B内核栈的栈顶

    uint32_t sp1 = sp0 - sizeof(struct trapframe);  //分配一个trapframe的空间
    struct trapframe* tf = (struct trapframe*)sp1;
    memset(tf, 0, sizeof(struct trapframe));
    //tf->status = MSTATUS_MIE;
    tf->status = 0x00001880;  // MPP为3， 表明来自M态，MPIE为1
    tf->epc = &taskB;  //中断返回后开始执行taskB
    tf->gpr.sp = sp0;  //设置B的栈为其内核栈

    struct pushregs* context = (struct pushregs*)&taskB_context;
    memset(context, 0, sizeof(struct pushregs));
    context->ra = &__trapret;  //B在上下文切换后执行__trapret， __trapret进行中断返回，中断返回后执行taskB
    context->sp = sp1;  //__trapret返回时栈指针指向trapframe
    

    //while (1);
    current = 1;
    taskA();

    

    htif_poweroff();
}
