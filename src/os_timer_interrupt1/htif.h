/*hitf提供了内核一些基本的输入输出接口和需要的底层功能。
 */

#ifndef _RISCV_HTIF_H
#define _RISCV_HTIF_H

#include <stdint.h>

void htif_console_putchar(uint8_t ch);//输出一个字符

void htif_poweroff();//关机

void putstring(const char *s);//输出一个字符串

void put_uint64(uint64_t x);//输出64位整数


#endif