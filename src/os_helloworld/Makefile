

CC = riscv32-unknown-elf-gcc
OBJCOPY = riscv32-unknown-elf-objcopy
OBJDUMP = riscv32-unknown-elf-objdump
QEMU = qemu-system-riscv32

CFLAGS = -MMD -MP -Wall -D__NO_INLINE__ -mcmodel=medany -O2 -std=gnu99 -Wno-unused -Wno-attributes -fno-delete-null-pointer-checks  -I. 
LDFLAGS = -nostartfiles -nostdlib -static -lgcc -T bbl.lds  
OBJS = bbl.o mentry.o

.PHONY : qemu riscvemu objdump clean

qemu  : bbl
	$(QEMU)  -machine spike_v1.10 -kernel bbl -nographic

bbl : $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $(OBJS)

bbl.o:bbl.c
	$(CC) $(CFLAGS) -c $<
mentry.o:mentry.S
	$(CC) $(CFLAGS) -c $<

objdump : bbl
	$(OBJDUMP) -S $< > bbl.s


clean:
	rm -f *.o *.d bbl bbl.bin bbl.s


