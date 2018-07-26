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

void put_hex(uint8_t c)
{
        if  (c <= 9)
            htif_console_putchar(c + '0');
        else if  (c <= 15)
            htif_console_putchar(c - 10 + 'a');
        else
            htif_console_putchar('x');

}

void put_uint64(uint64_t x)
{
    uint8_t* s = (uint8_t*)&x;
    for (int i = 7; i > 0; i--)
    {
        uint8_t c = s[i];
        put_hex((c >> 4) & 0xf);
        put_hex(c & 0xf);
    }
    htif_console_putchar('\n');
}


extern void __alltraps(void);

uint64_t* mtime_p = (uint64_t*)0x00200bff8;
uint64_t* mtimecmp_p = (uint64_t*)0x02004000;
uint64_t timebase = 0x1000000;

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


void trap(void)
{

    //++mtime;
    //putstring("trap\n");
    put_uint64(get_mtime());
    //mtimecmp += 5000000;
    //set_mtimecmp(mtimecmp);
    set_mtimecmp(mtimecmp += timebase);

    /*if (mtime == mtimecmp)
    {
        mtimecmp += 500000;
        putstring("ticks\n");
    }*/
}


void boot_loader()
{
    putstring("hello world\n");
    put_uint64(get_mtime());

    mtimecmp = timebase;
    set_mtimecmp(mtimecmp);

    asm volatile("csrw mie,%0" ::"rK"(0x80));
    asm volatile("csrw mtvec,%0" ::"rK"(&__alltraps));
    asm volatile("csrw mstatus,%0" ::"rK"(0x8));
    
    /*while (1)
    {   //uint64_t cycles = get_cycles();
        putstring("cycles\n");
        //char* str = cycles;
        //str[7] = '\n';
        //putstring(str);
            
    }*/
    while (1);


    htif_poweroff();
}
