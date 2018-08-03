#include <stdint.h>
#include "htif.h"
#include "encoding.h"


extern void __alltraps(void);

uint64_t* mtime_p = (uint64_t*)0x00200bff8;  //mtime寄存器在内存中映射的地址
uint64_t* mtimecmp_p = (uint64_t*)0x02004000;  //mtimecmp寄存器在内存中映射的地址
uint64_t timebase = 0x1000000;
uint64_t mtimecmp;

uint64_t get_mtime()
{
    return *mtime_p;
}

void set_mtimecmp(uint64_t when)  //设置mtimecmp，为了防止设置时因为中间值产生新的中断，使用了一些特殊的写法，详情参见riscv-privileged-v1.10
{
    uint32_t* src = (uint32_t*)&when;
    uint32_t* dst = (uint32_t*)mtimecmp_p;
    dst[0] = -1;
    dst[1] = src[1];
    dst[0] = src[0];
}


void trap(void)  //中断处理函数
{

    put_uint64(get_mtime());  //打印当前时间
    set_mtimecmp(mtimecmp += timebase);  //设置下一次的中断时间

}


void kern_entry()
{
    putstring("hello world\n");
    put_uint64(get_mtime());

    mtimecmp = timebase;
    set_mtimecmp(mtimecmp);

    write_csr(mtvec, &__alltraps);  //设置系统的中断处理函数
    write_csr(mie, MIP_MTIP);  //开启M态的时钟中断
    write_csr(mstatus, MSTATUS_MIE);  //允许M态的中断，这是M态中断的总开关
    

    while (1);

    htif_poweroff();  //正常情况下，这条语句不应该被执行
}
