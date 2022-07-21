.section .boot, "ax", @progbits
.global _start

_start:
	li x1, 0x001122330
	addi x2, x1, 55

