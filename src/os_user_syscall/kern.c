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


#define SYS_PUTC 0x12345678

uint32_t sys_putc(uint32_t c)
{
    register uint32_t a7 asm ("a7") = SYS_PUTC;  // syscall index
	register uint32_t a1 asm ("a1") = c;	
    register uint32_t ret asm ("a0") = -1;

	asm volatile ("ecall"
		          : "+r" (ret)
		          : "r"(a1), "r"(a7)
		          : "memory");
    return ret;
}

void taskA()
{
    //while (1)  putstring("A");
    while (1)  sys_putc('A');
    //while (1)  put_uint32(sys_putc('A'));
}

void taskB()
{
    //while (1)  putstring("B");
    while (1)  sys_putc('B');
}







void trap(struct trapframe* tf)
{
    //put_uint32(tf->status);
    //put_uint32(tf->cause);

    if  (tf->cause == (0x80000000 | IRQ_M_TIMER))
    //if  (0)
    {
        putstring("\n");
        put_uint64(get_mtime());
        put_uint32(tf->status);
        put_uint32(tf->cause);
        put_uint32(current);
        //putstring("clock interrupt\n");
        
        set_mtimecmp(mtimecmp += timebase);
        
        if  (current == 1)
        {   current = 0;
            switch_to(&taskA_context, &taskB_context);
        }
        else
        {   current = 1;
            switch_to(&taskB_context, &taskA_context);
        }
    }
    else if  (tf->cause == CAUSE_USER_ECALL)
    {
        /*putstring("syscall\n");
        for (int i = 0; i < 31; i++)
            put_uint32(((uint32_t*)&tf->gpr)[i]);*/
        
        tf->epc += 4;
        
        uint32_t n = tf->gpr.a7;
        uint32_t a1 = tf->gpr.a1;

        if  (n == SYS_PUTC)
        {   htif_console_putchar((uint8_t)a1);
            tf->gpr.a0 = 0x87654321;
        }
        else 
        {   putstring("bad syscall:  ");
            put_uint32(n);
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
    



    uintptr_t mstatus = 0;
    mstatus = INSERT_FIELD(mstatus, MSTATUS_MPP, PRV_U);
    put_uint32(mstatus);
    //mstatus = INSERT_FIELD(mstatus, MSTATUS_MPIE, 1);
    //mstatus = INSERT_FIELD(mstatus, MSTATUS_UIE, 1);
    put_uint32(mstatus);

    
    uint32_t taskB_sp0 = (uint32_t)(taskB_kern_stack + KERN_STACK_SIZE);

    uint32_t taskB_sp1 = taskB_sp0 - sizeof(struct trapframe);
    struct trapframe* taskB_tf = (struct trapframe*)taskB_sp1;
    memset(taskB_tf, 0, sizeof(struct trapframe));
    taskB_tf->status = mstatus;
    taskB_tf->epc = (uint32_t)&taskB;
    taskB_tf->gpr.sp = (uint32_t)(taskB_user_stack + USER_STACK_SIZE);

    memset(&taskB_context, 0, sizeof(struct pushregs));
    taskB_context.ra = (uint32_t)&__trapret;
    taskB_context.sp = taskB_sp1;
    


    uint32_t taskA_sp0 = (uint32_t)(taskA_kern_stack + KERN_STACK_SIZE);

    uint32_t taskA_sp1 = taskA_sp0 - sizeof(struct trapframe);
    struct trapframe* taskA_tf = (struct trapframe*)taskA_sp1;
    memset(taskA_tf, 0, sizeof(struct trapframe));
    taskA_tf->status = mstatus;
    taskA_tf->epc = (uint32_t)&taskA;
    taskA_tf->gpr.sp = (uint32_t)(taskA_user_stack + USER_STACK_SIZE);



    current = 1;
    asm volatile("mv sp, %0 \n\t"
                 "j __trapret \n\t"
                 :
                 : "r"(taskA_sp1));
        

    htif_poweroff();
}
