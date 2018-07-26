#include <stdint.h>
#include "htif.h"
#include "encoding.h"
#include "trap.h"


extern void __alltraps(void);

uint64_t* mtime_p = (uint64_t*)0x00200bff8;
uint64_t* mtimecmp_p = (uint64_t*)0x02004000;
uint64_t timebase = 0x1000000;
uint64_t mtimecmp;

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


void trap(struct trapframe* tf)
{

    put_uint64(get_mtime());
    put_uint32(tf->cause);
    set_mtimecmp(mtimecmp += timebase);

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
    

    while (1);

    htif_poweroff();
}
