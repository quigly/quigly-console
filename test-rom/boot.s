.section .text

.globl _start
.globl _init
.globl _update
.globl _draw
.globl camera
.globl pget
.globl pset

_start:
	jal ra, _init
	la t0, _gameloop
	csrw 0x7C2, t0
	li a7, 98
	ecall
	ebreak

_gameloop:
	addi sp, sp, -16
	sw ra, 12(sp)
	
	call _update
	call _draw
	li a7, 99
	ecall
	
	lw ra, 12(sp)
	addi sp, sp, 16

	ret

camera:
	li a7, 100
	ecall
	ret

pget:
	li a7, 101
	ecall
	ret

pset:
	li a7, 102
	ecall
	ret
