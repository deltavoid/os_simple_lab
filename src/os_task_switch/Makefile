

CC = riscv32-unknown-elf-gcc
OBJCOPY = riscv32-unknown-elf-objcopy
OBJDUMP = riscv32-unknown-elf-objdump
QEMU = qemu-system-riscv32

CFLAGS = -MMD -MP -Wall -D__NO_INLINE__ -mcmodel=medany -O2 -std=gnu99 -Wno-unused -Wno-attributes -fno-delete-null-pointer-checks  -I. 
LDFLAGS = -nostartfiles -nostdlib -static -lgcc -T kern.lds  
OBJS = kern.o entry.o htif.o

.PHONY : qemu objdump clean

qemu  : kern
	$(QEMU)  -machine spike_v1.10  -kernel kern -nographic > log.txt

kern : $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $(OBJS)

kern.o:kern.c
	$(CC) $(CFLAGS) -c $<
entry.o: entry.S
	$(CC) $(CFLAGS) -c $<
htif.o : htif.c
	$(CC) $(CFLAGS) -c $<

objdump : kern
	$(OBJDUMP) -S $< > kern.s


clean:
	rm -f *.o *.d kern  kern.s


