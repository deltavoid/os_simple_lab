#include <stdint.h>

volatile extern uint64_t tohost __attribute__((section(".htif")));
volatile extern uint64_t fromhost __attribute__((section(".htif")));
volatile int htif_console_buf;

#define TOHOST_CMD(dev, cmd, payload) \
    (((uint64_t)(dev) << 56) | ((uint64_t)(cmd) << 48) | (uint64_t)(payload))

#define FROMHOST_CMD(fromhost_value) ((uint64_t)(fromhost_value) << 8 >> 56)
#define FROMHOST_DATA(fromhost_value) ((uint64_t)(fromhost_value) << 16 >> 16)

void htif_console_putchar(uint8_t ch)
{
    while (tohost)
    {
        if (0 == FROMHOST_CMD(fromhost))
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

void boot_loader(uintptr_t dtb)
{
    putstring("Hello World!\n");
    htif_poweroff();
}
