#ifndef _RISCV_HTIF_H
#define _RISCV_HTIF_H

#include <stdint.h>

void htif_console_putchar(uint8_t ch);
void htif_poweroff();
void putstring(const char *s);

void put_uint64(uint64_t x);
void put_uint32(uint32_t x);


#endif