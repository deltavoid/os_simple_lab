#define TOHOST_CMD(dev, cmd, payload) \
  (((uint64_t)(dev) << 56) | ((uint64_t)(cmd) << 48) | (uint64_t)(payload))
#define FROMHOST_CMD(fromhost_value) ((uint64_t)(fromhost_value) << 8 >> 56)
#define FROMHOST_DATA(fromhost_value) ((uint64_t)(fromhost_value) << 16 >> 16)

typedef unsigned char uint8_t;
typedef unsigned int uint32_t;
typedef unsigned long long int uint64_t;
typedef uint32_t uintptr_t;

volatile extern uint64_t tohost;
volatile extern uint64_t fromhost;
volatile int htif_console_buf;

extern void __alltraps(void);

struct trapframe{
  uintptr_t x0;		// Hard-wired zero
  uintptr_t x1;		// ra
  uintptr_t x2;		// sp
  uintptr_t x3;		// gp
  uintptr_t x4;		// tp
  uintptr_t x5;		// t0
  uintptr_t x6;		// t1
  uintptr_t x7;		// s0
  uintptr_t x8;		// s1
  uintptr_t x9;		// s2
  uintptr_t x10;	// a0
  uintptr_t x11;	// a1
  uintptr_t x12;	// a2
  uintptr_t x13;	// a3
  uintptr_t x14;	// a4
  uintptr_t x15;	// a5
  uintptr_t x16;	// a6
  uintptr_t x17;	// a7
  uintptr_t x18;	// s2
  uintptr_t x19;	// s3
  uintptr_t x20;	// s4
  uintptr_t x21;	// s5
  uintptr_t x22;	// s6
  uintptr_t x23;	// s7
  uintptr_t x24;	// s8
  uintptr_t x25;	// s9
  uintptr_t x26;	// s10
  uintptr_t x27;	// s11
  uintptr_t x28;	// t3
  uintptr_t x29;	// t4
  uintptr_t x30;	// t5
  uintptr_t x31;	// t6
  uintptr_t cause;
  uintptr_t status;
  uintptr_t tvec;
  uintptr_t epc;
};


void htif_console_putchar(uint8_t ch)
{
	  while (tohost){
		  if (!FROMHOST_CMD(fromhost)) {
		      htif_console_buf = 1 + (uint8_t)FROMHOST_DATA(fromhost);
		  }
	  }
	  tohost = TOHOST_CMD(1, 1, ch);
}

void htif_poweroff()
{
  while (1) {
    fromhost = 0;
    tohost = 1;
  }
}
void putstring(const char* s)
{
    int c;
    while (*s) {
        c = *s++;
        if (c == '\n')
            htif_console_putchar('\r');
        htif_console_putchar(c);
    }
}


void printn(uintptr_t cause){
  
  if(cause & 0x80000000) putstring("1");else putstring("0");
  if(cause & 0x40000000) putstring("1");else putstring("0");
  if(cause & 0x20000000) putstring("1");else putstring("0");
  if(cause & 0x10000000) putstring("1");else putstring("0");
  if(cause & 0x8000000) putstring("1");else putstring("0");
  if(cause & 0x4000000) putstring("1");else putstring("0");
  if(cause & 0x2000000) putstring("1");else putstring("0");
  if(cause & 0x1000000) putstring("1 ");else putstring("0 ");
  if(cause & 0x800000) putstring("1");else putstring("0");
  if(cause & 0x400000) putstring("1");else putstring("0");
  if(cause & 0x200000) putstring("1");else putstring("0");
  if(cause & 0x100000) putstring("1");else putstring("0");
  if(cause & 0x80000) putstring("1");else putstring("0");
  if(cause & 0x40000) putstring("1");else putstring("0");
  if(cause & 0x20000) putstring("1");else putstring("0");
  if(cause & 0x10000) putstring("1 ");else putstring("0 ");
  if(cause & 0x8000) putstring("1");else putstring("0");
  if(cause & 0x4000) putstring("1");else putstring("0");
  if(cause & 0x2000) putstring("1");else putstring("0");
  if(cause & 0x1000) putstring("1");else putstring("0");
  if(cause & 0x800) putstring("1");else putstring("0");
  if(cause & 0x400) putstring("1");else putstring("0");
  if(cause & 0x200) putstring("1");else putstring("0");
  if(cause & 0x100) putstring("1 ");else putstring("0 ");
  if(cause & 0x80) putstring("1");else putstring("0");
  if(cause & 0x40) putstring("1");else putstring("0");
  if(cause & 0x20) putstring("1");else putstring("0");
  if(cause & 0x10) putstring("1");else putstring("0");
  if(cause & 0x8) putstring("1");else putstring("0");
  if(cause & 0x4) putstring("1");else putstring("0");
  if(cause & 0x2) putstring("1");else putstring("0");
  if(cause & 0x1) putstring("1");else putstring("0");
  putstring("\n");
}

void trap(struct trapframe *tf){
  int cause = (int)tf->cause;
/* szx
  putstring("x0: ");printn(tf->x0);
  putstring("x1: ");printn(tf->x1);
  putstring("x2: ");printn(tf->x2);
  putstring("x3: ");printn(tf->x3);
  putstring("x4: ");printn(tf->x4);
  putstring("x5: ");printn(tf->x5);
  putstring("x6: ");printn(tf->x6);
  putstring("x7: ");printn(tf->x7);
  putstring("x8: ");printn(tf->x8);
  putstring("x9: ");printn(tf->x9);
  putstring("x10: ");printn(tf->x10);
  putstring("x11: ");printn(tf->x11);
  putstring("x12: ");printn(tf->x12);
  putstring("x13: ");printn(tf->x13);
  putstring("x14: ");printn(tf->x14);
  putstring("x15: ");printn(tf->x15);
  putstring("x16: ");printn(tf->x16);
  putstring("x17: ");printn(tf->x17);
  putstring("x18: ");printn(tf->x18);
  putstring("x19: ");printn(tf->x19);
  putstring("x20: ");printn(tf->x20);
  putstring("x21: ");printn(tf->x21);
  putstring("x22: ");printn(tf->x22);
  putstring("x23: ");printn(tf->x23);
  putstring("x24: ");printn(tf->x24);
  putstring("x25: ");printn(tf->x25);
  putstring("x26: ");printn(tf->x26);
  putstring("x27: ");printn(tf->x27);
  putstring("x28: ");printn(tf->x28);
  putstring("x29: ");printn(tf->x29);
  putstring("x30: ");printn(tf->x30);
  putstring("x31: ");printn(tf->x31);
  putstring("cause: ");printn(tf->cause);
szx */
  if(cause < 0){
    putstring("interrupt happens\n");
  }else{
    switch(cause){
      case 0: putstring("instruction misaligned.\n");break;
      case 1: putstring("instruction access fault.\n"); break;
      case 2: putstring("illegal instruction.\n"); break;
      case 3: putstring("break point.\n"); break;
      case 4: putstring("load misaligned.\n"); break;
      case 5: putstring("load access fault.\n"); break;
      case 6: putstring("store/AMO misaligned.\n"); break;
      case 7: putstring("Store/AMO access fault.\n"); break;
      case 8: putstring("ecall from U-mode.\n"); break;
      case 9: putstring("ecall from S-mode.\n"); break;
      case 11: putstring("ecall from M-mode.\n"); break;
      case 12: putstring("instruction page fault.\n"); break;
      case 13: putstring("load page fault.\n"); break;
      case 15: putstring("store/AMO page fault.\n"); break;
      default: putstring(" other exception happens\n"); break;
    }
  }
}

void boot_loader(uintptr_t dtb)
{
  asm volatile("csrw mie,%0"::"rK"(0x888));
  asm volatile("csrw mtvec,%0"::"rK"(&__alltraps));
  asm volatile("csrw mstatus,%0"::"rK"(0x8));
  while(1);
  htif_poweroff();
}
