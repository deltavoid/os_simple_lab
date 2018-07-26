#include "htif.h"

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
    for (int i = 7; i >= 0; i--)
    {
        uint8_t c = s[i];
        put_hex((c >> 4) & 0xf);
        put_hex(c & 0xf);
    }
    htif_console_putchar('\n');
}

void put_uint32(uint32_t x)
{
    uint8_t* s = (uint8_t*)&x;
    for (int i = 3; i >= 0; i--)
    {
        uint8_t c = s[i];
        put_hex((c >> 4) & 0xf);
        put_hex(c & 0xf);
    }
    htif_console_putchar('\n');
}