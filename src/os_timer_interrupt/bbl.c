#include <stdint.h>

volatile uint64_t tohost __attribute__((section(".htif")));
volatile uint64_t fromhost __attribute__((section(".htif")));
volatile int htif_console_buf;
uint64_t mtime, mtimecmp;

#define TOHOST_CMD(dev, cmd, payload) \
    (((uint64_t)(dev) << 56) | ((uint64_t)(cmd) << 48) | (uint64_t)(payload))

#define FROMHOST_CMD(fromhost_value) ((uint64_t)(fromhost_value) << 8 >> 56)
#define FROMHOST_DATA(fromhost_value) ((uint64_t)(fromhost_value) << 16 >> 16)


void htif_console_putchar(uint8_t ch)
{
    while (tohost)
    {
        if (!FROMHOST_CMD(fromhost))
        {
            htif_console_buf = 1 + (uint8_t)FROMHOST_DATA(fromhost);
        }
    }
    tohost = TOHOST_CMD(1, 1, ch);
}

void htif_poweroff()
{
    while (1)
    {
        fromhost = 0;
        tohost = 1;
    }
}

void putstring(const char *s)
{
    int c;
    while (*s)
    {
        c = *s++;
        if (c == '\n')
            htif_console_putchar('\r');
        htif_console_putchar(c);
    }
}


extern void __alltraps(void);


/*
void set_time_cmp(uint64_t when)
{
    asm volatile ("li t0, -1\n\t"
                  "sw 
    );
}*/


void trap(void)
{

    ++mtime;
    //putstring("ticks\n");

    if (mtime == mtimecmp)
    {
        mtimecmp += 5000;
        putstring("ticks\n");
    }
}



static inline uint64_t get_cycles(void) {
#if __riscv_xlen == 64
    uint64_t n;
    __asm__ __volatile__("rdtime %0" : "=r"(n));
    return n;
#else
    uint32_t lo, hi, tmp;
    __asm__ __volatile__(
        "1:\n"
        "rdtimeh %0\n"
        "rdtime %1\n"
        "rdtimeh %2\n"
        "bne %0, %2, 1b"
        : "=&r"(hi), "=&r"(lo), "=&r"(tmp));
    return ((uint64_t)hi << 32) | lo;
#endif
}

void boot_loader()
{
    putstring("hello world\n");

    mtime = 0;
    mtimecmp = 5000000;
    asm volatile("csrw mie,%0" ::"rK"(0x80));
    asm volatile("csrw mip,%0" ::"rK"(0x80));
    asm volatile("csrw mtvec,%0" ::"rK"(&__alltraps));
    asm volatile("csrw mstatus,%0" ::"rK"(0x8));
    
    while (1)
    {   uint64_t cycles = get_cycles();
        char* str = cycles;
        str[7] = '\n';
        //putstring(str);
            
    }
    htif_poweroff();
}
