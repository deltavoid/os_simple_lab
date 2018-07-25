#include <stdint.h>

volatile extern uint64_t tohost;
volatile extern uint64_t fromhost;
volatile int htif_console_buf;
uint64_t mtime, mtimecmp;

#define TOHOST_CMD(dev, cmd, payload) \
    (((uint64_t)(dev) << 56) | ((uint64_t)(cmd) << 48) | (uint64_t)(payload))

#define FROMHOST_CMD(fromhost_value) ((uint64_t)(fromhost_value) << 8 >> 56)
#define FROMHOST_DATA(fromhost_value) ((uint64_t)(fromhost_value) << 16 >> 16)

extern void __alltraps(void);
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

void trap(void)
{

    ++mtime;

    if (mtime == mtimecmp)
    {
        mtimecmp += 5000000;
        putstring("ticks\n");
    }
}

void boot_loader(uintptr_t dtb)
{
    mtime = 0;
    mtimecmp = 5000000;
    asm volatile("csrw mie,%0" ::"rK"(0x80));
    asm volatile("csrw mtvec,%0" ::"rK"(&__alltraps));
    asm volatile("csrw mstatus,%0" ::"rK"(0x8));
    htif_poweroff();
}
