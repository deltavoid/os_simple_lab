
bbl：     文件格式 elf32-littleriscv


Disassembly of section .text:

80000000 <_ftext>:
80000000:	00003117          	auipc	sp,0x3
80000004:	00010113          	mv	sp,sp
80000008:	2b80006f          	j	800002c0 <boot_loader>

8000000c <__alltraps>:
8000000c:	34011073          	csrw	mscratch,sp
80000010:	f7810113          	addi	sp,sp,-136 # 80002f78 <stacks+0xf78>
80000014:	00012023          	sw	zero,0(sp)
80000018:	00112223          	sw	ra,4(sp)
8000001c:	00312623          	sw	gp,12(sp)
80000020:	00412823          	sw	tp,16(sp)
80000024:	00512a23          	sw	t0,20(sp)
80000028:	00612c23          	sw	t1,24(sp)
8000002c:	00712e23          	sw	t2,28(sp)
80000030:	02812023          	sw	s0,32(sp)
80000034:	02912223          	sw	s1,36(sp)
80000038:	02a12423          	sw	a0,40(sp)
8000003c:	02b12623          	sw	a1,44(sp)
80000040:	02c12823          	sw	a2,48(sp)
80000044:	02d12a23          	sw	a3,52(sp)
80000048:	02e12c23          	sw	a4,56(sp)
8000004c:	02f12e23          	sw	a5,60(sp)
80000050:	05012023          	sw	a6,64(sp)
80000054:	05112223          	sw	a7,68(sp)
80000058:	05212423          	sw	s2,72(sp)
8000005c:	05312623          	sw	s3,76(sp)
80000060:	05412823          	sw	s4,80(sp)
80000064:	05512a23          	sw	s5,84(sp)
80000068:	05612c23          	sw	s6,88(sp)
8000006c:	05712e23          	sw	s7,92(sp)
80000070:	07812023          	sw	s8,96(sp)
80000074:	07912223          	sw	s9,100(sp)
80000078:	07a12423          	sw	s10,104(sp)
8000007c:	07b12623          	sw	s11,108(sp)
80000080:	07c12823          	sw	t3,112(sp)
80000084:	07d12a23          	sw	t4,116(sp)
80000088:	07e12c23          	sw	t5,120(sp)
8000008c:	07f12e23          	sw	t6,124(sp)
80000090:	34005473          	csrrwi	s0,mscratch,0
80000094:	300024f3          	csrr	s1,mstatus
80000098:	34102973          	csrr	s2,mepc
8000009c:	00812423          	sw	s0,8(sp)
800000a0:	08912023          	sw	s1,128(sp)
800000a4:	09212223          	sw	s2,132(sp)
800000a8:	00010513          	mv	a0,sp
800000ac:	19c000ef          	jal	ra,80000248 <trap>

800000b0 <__trapret>:
800000b0:	08012483          	lw	s1,128(sp)
800000b4:	08412903          	lw	s2,132(sp)
800000b8:	30049073          	csrw	mstatus,s1
800000bc:	34191073          	csrw	mepc,s2
800000c0:	00412083          	lw	ra,4(sp)
800000c4:	00c12183          	lw	gp,12(sp)
800000c8:	01012203          	lw	tp,16(sp)
800000cc:	01412283          	lw	t0,20(sp)
800000d0:	01812303          	lw	t1,24(sp)
800000d4:	01c12383          	lw	t2,28(sp)
800000d8:	02012403          	lw	s0,32(sp)
800000dc:	02412483          	lw	s1,36(sp)
800000e0:	02812503          	lw	a0,40(sp)
800000e4:	02c12583          	lw	a1,44(sp)
800000e8:	03012603          	lw	a2,48(sp)
800000ec:	03412683          	lw	a3,52(sp)
800000f0:	03812703          	lw	a4,56(sp)
800000f4:	03c12783          	lw	a5,60(sp)
800000f8:	04012803          	lw	a6,64(sp)
800000fc:	04412883          	lw	a7,68(sp)
80000100:	04812903          	lw	s2,72(sp)
80000104:	04c12983          	lw	s3,76(sp)
80000108:	05012a03          	lw	s4,80(sp)
8000010c:	05412a83          	lw	s5,84(sp)
80000110:	05812b03          	lw	s6,88(sp)
80000114:	05c12b83          	lw	s7,92(sp)
80000118:	06012c03          	lw	s8,96(sp)
8000011c:	06412c83          	lw	s9,100(sp)
80000120:	06812d03          	lw	s10,104(sp)
80000124:	06c12d83          	lw	s11,108(sp)
80000128:	07012e03          	lw	t3,112(sp)
8000012c:	07412e83          	lw	t4,116(sp)
80000130:	07812f03          	lw	t5,120(sp)
80000134:	07c12f83          	lw	t6,124(sp)
80000138:	00812103          	lw	sp,8(sp)
8000013c:	30200073          	mret

80000140 <htif_console_putchar>:
80000140:	c0008697          	auipc	a3,0xc0008
80000144:	ec068693          	addi	a3,a3,-320 # 40008000 <tohost>
80000148:	c0008617          	auipc	a2,0xc0008
8000014c:	ec060613          	addi	a2,a2,-320 # 40008008 <fromhost>
80000150:	0180006f          	j	80000168 <htif_console_putchar+0x28>
80000154:	00062703          	lw	a4,0(a2)
80000158:	00462783          	lw	a5,4(a2)
8000015c:	00879793          	slli	a5,a5,0x8
80000160:	0187d793          	srli	a5,a5,0x18
80000164:	02078263          	beqz	a5,80000188 <htif_console_putchar+0x48>
80000168:	0006a703          	lw	a4,0(a3)
8000016c:	0046a783          	lw	a5,4(a3)
80000170:	00f76733          	or	a4,a4,a5
80000174:	fe0710e3          	bnez	a4,80000154 <htif_console_putchar+0x14>
80000178:	010107b7          	lui	a5,0x1010
8000017c:	00a6a023          	sw	a0,0(a3)
80000180:	00f6a223          	sw	a5,4(a3)
80000184:	00008067          	ret
80000188:	00062703          	lw	a4,0(a2)
8000018c:	00462783          	lw	a5,4(a2)
80000190:	0ff77713          	andi	a4,a4,255
80000194:	00170713          	addi	a4,a4,1
80000198:	00003797          	auipc	a5,0x3
8000019c:	e6e7ac23          	sw	a4,-392(a5) # 80003010 <htif_console_buf>
800001a0:	fc9ff06f          	j	80000168 <htif_console_putchar+0x28>

800001a4 <htif_poweroff>:
800001a4:	c0008617          	auipc	a2,0xc0008
800001a8:	e6460613          	addi	a2,a2,-412 # 40008008 <fromhost>
800001ac:	c0008697          	auipc	a3,0xc0008
800001b0:	e5468693          	addi	a3,a3,-428 # 40008000 <tohost>
800001b4:	00100713          	li	a4,1
800001b8:	00000793          	li	a5,0
800001bc:	00000513          	li	a0,0
800001c0:	00a62023          	sw	a0,0(a2)
800001c4:	00000593          	li	a1,0
800001c8:	00b62223          	sw	a1,4(a2)
800001cc:	00e6a023          	sw	a4,0(a3)
800001d0:	00f6a223          	sw	a5,4(a3)
800001d4:	fe9ff06f          	j	800001bc <htif_poweroff+0x18>

800001d8 <putstring>:
800001d8:	ff010113          	addi	sp,sp,-16
800001dc:	00112623          	sw	ra,12(sp)
800001e0:	00812423          	sw	s0,8(sp)
800001e4:	00912223          	sw	s1,4(sp)
800001e8:	01212023          	sw	s2,0(sp)
800001ec:	00054403          	lbu	s0,0(a0)
800001f0:	04040063          	beqz	s0,80000230 <putstring+0x58>
800001f4:	00050493          	mv	s1,a0
800001f8:	00a00913          	li	s2,10
800001fc:	0140006f          	j	80000210 <putstring+0x38>
80000200:	00040513          	mv	a0,s0
80000204:	f3dff0ef          	jal	ra,80000140 <htif_console_putchar>
80000208:	0004c403          	lbu	s0,0(s1)
8000020c:	02040263          	beqz	s0,80000230 <putstring+0x58>
80000210:	00148493          	addi	s1,s1,1
80000214:	ff2416e3          	bne	s0,s2,80000200 <putstring+0x28>
80000218:	00d00513          	li	a0,13
8000021c:	f25ff0ef          	jal	ra,80000140 <htif_console_putchar>
80000220:	00040513          	mv	a0,s0
80000224:	f1dff0ef          	jal	ra,80000140 <htif_console_putchar>
80000228:	0004c403          	lbu	s0,0(s1)
8000022c:	fe0412e3          	bnez	s0,80000210 <putstring+0x38>
80000230:	00c12083          	lw	ra,12(sp)
80000234:	00812403          	lw	s0,8(sp)
80000238:	00412483          	lw	s1,4(sp)
8000023c:	00012903          	lw	s2,0(sp)
80000240:	01010113          	addi	sp,sp,16
80000244:	00008067          	ret

80000248 <trap>:
80000248:	00003717          	auipc	a4,0x3
8000024c:	dc070713          	addi	a4,a4,-576 # 80003008 <mtime>
80000250:	00072783          	lw	a5,0(a4)
80000254:	00472603          	lw	a2,4(a4)
80000258:	00003597          	auipc	a1,0x3
8000025c:	da858593          	addi	a1,a1,-600 # 80003000 <mtimecmp>
80000260:	00178693          	addi	a3,a5,1
80000264:	00f6b733          	sltu	a4,a3,a5
80000268:	0005a503          	lw	a0,0(a1)
8000026c:	00c70733          	add	a4,a4,a2
80000270:	00003817          	auipc	a6,0x3
80000274:	d8d82c23          	sw	a3,-616(a6) # 80003008 <mtime>
80000278:	00003817          	auipc	a6,0x3
8000027c:	d8e82a23          	sw	a4,-620(a6) # 8000300c <mtime+0x4>
80000280:	00d50463          	beq	a0,a3,80000288 <trap+0x40>
80000284:	00008067          	ret
80000288:	0045a683          	lw	a3,4(a1)
8000028c:	fee69ce3          	bne	a3,a4,80000284 <trap+0x3c>
80000290:	004c5737          	lui	a4,0x4c5
80000294:	b4170713          	addi	a4,a4,-1215 # 4c4b41 <tohost-0x3fb434bf>
80000298:	00e78733          	add	a4,a5,a4
8000029c:	00f737b3          	sltu	a5,a4,a5
800002a0:	00c787b3          	add	a5,a5,a2
800002a4:	00003697          	auipc	a3,0x3
800002a8:	d4e6ae23          	sw	a4,-676(a3) # 80003000 <mtimecmp>
800002ac:	00000517          	auipc	a0,0x0
800002b0:	06c50513          	addi	a0,a0,108 # 80000318 <boot_loader+0x58>
800002b4:	00003717          	auipc	a4,0x3
800002b8:	d4f72823          	sw	a5,-688(a4) # 80003004 <mtimecmp+0x4>
800002bc:	f1dff06f          	j	800001d8 <putstring>

800002c0 <boot_loader>:
800002c0:	ff010113          	addi	sp,sp,-16
800002c4:	00112623          	sw	ra,12(sp)
800002c8:	00003797          	auipc	a5,0x3
800002cc:	d4078793          	addi	a5,a5,-704 # 80003008 <mtime>
800002d0:	00000693          	li	a3,0
800002d4:	00000713          	li	a4,0
800002d8:	004c5637          	lui	a2,0x4c5
800002dc:	00d7a023          	sw	a3,0(a5)
800002e0:	00e7a223          	sw	a4,4(a5)
800002e4:	b4060613          	addi	a2,a2,-1216 # 4c4b40 <tohost-0x3fb434c0>
800002e8:	00003797          	auipc	a5,0x3
800002ec:	d1878793          	addi	a5,a5,-744 # 80003000 <mtimecmp>
800002f0:	00000693          	li	a3,0
800002f4:	00c7a023          	sw	a2,0(a5)
800002f8:	00d7a223          	sw	a3,4(a5)
800002fc:	08000793          	li	a5,128
80000300:	30479073          	csrw	mie,a5
80000304:	00000797          	auipc	a5,0x0
80000308:	d0878793          	addi	a5,a5,-760 # 8000000c <__alltraps>
8000030c:	30579073          	csrw	mtvec,a5
80000310:	30045073          	csrwi	mstatus,8
80000314:	e91ff0ef          	jal	ra,800001a4 <htif_poweroff>
