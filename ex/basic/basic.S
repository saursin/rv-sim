.section .boot, "ax", @progbits
.global _start

_start:
	li x1, 0x001122335
	
	addi x2, x1, 0x551
	andi x3, x1, 0x551
	ori x4, x1, 0x551
	xori x5, x1, 0x551

