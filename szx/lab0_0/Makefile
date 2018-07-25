CC = riscv32-unknown-elf-gcc
OBJCOPY = riscv32-unknown-elf-objcopy
OBJDUMP = riscv32-unknown-elf-objdump
QEMU = qemu-system-riscv32

CFLAGS1 = -MMD -MP -Wall -D__NO_INLINE__ -mcmodel=medany -O2 -std=gnu99 -Wno-unused -Wno-attributes -fno-delete-null-pointer-checks  -I. 
CFLAGS2 = -nostartfiles -nostdlib -static -lgcc -T bbl.lds  
OBJS = bbl.o mentry.o


all:bbl
bbl.o:bbl.c
	$(CC) $(CFLAGS1) -c $<
mentry.o:mentry.S
	$(CC) $(CFLAGS1) -c $<
bbl:$(OBJS)
	$(CC) $(CFLAGS2) -o $@ $(OBJS)
all:bbl
	$(OBJCOPY) -O binary $< bbl.bin
	$(OBJDUMP) -S $< > bbl.s
run:all
	@riscvemu32 -ctrlc lab00.cfg
qemu:all
	qemu-system-riscv32  -machine spike_v1.10 -kernel bbl -nographic
clean:
	@rm -f *.o *.d bbl bbl.bin bbl.s
.PHONY:all run clean
