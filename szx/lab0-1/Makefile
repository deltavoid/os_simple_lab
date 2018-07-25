CC = riscv32-unknown-elf-gcc
OBJCOPY = riscv32-unknown-elf-objcopy
OBJDUMP = riscv32-unknown-elf-objdump
QEMU = qemu-system-riscv32

CFLAGS1 = -MMD -MP -Wall -Werror -D__NO_INLINE__ -mcmodel=medany -O2 -std=gnu99 -Wno-unused -Wno-attributes -fno-delete-null-pointer-checks  -I. 
CFLAGS2 = -nostartfiles -nostdlib -static -lgcc -T bbl.lds  
OBJS = bbl.o mentry.o

.PHONY:all run clean
all: bbl.bin
bbl.o:bbl.c
	$(CC) $(CFLAGS1) -c $<
mentry.o:mentry.S
	$(CC) $(CFLAGS1) -c $<
bbl:$(OBJS)
	$(CC) $(CFLAGS2) -o $@ $(OBJS)
bbl.bin:bbl
	$(OBJCOPY) -O binary $< $@
	$(OBJDUMP) -S $< > bbl.s
run:all
	@riscvemu32 -ctrlc lab00.cfg

qemu  : bbl
	$(QEMU)  -machine spike_v1.10 -kernel bbl -nographic

clean:
	@rm -f *.o *.d bbl bbl.bin bbl.s

