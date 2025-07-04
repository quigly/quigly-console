.globl _start
.globl _init
.globl _update
.globl _draw
.globl camera
.globl pget
.globl pset
.globl pal
.globl palt
.globl cls
.globl clip
.globl rect
.globl rectfill
.globl line
.globl spr
.globl print
.globl btn
.globl btnp
.globl putc
.globl shutdown

.section .start

_start:
	jal ra, _init
	la t0, _gameloop
	csrw 0x7C2, t0
	li a7, 98
	ecall
	ebreak

.section .text

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

pal:
	li a7, 103
	ecall
	ret

palt:
	li a7, 104
	ecall
	ret

cls:
	li a7, 105
	ecall
	ret

clip:
	li a7, 106
	ecall
	ret

rect:
	li a7, 107
	ecall
	ret

rectfill:
	li a7, 108
	ecall
	ret

line:
	li a7, 109
	ecall
	ret

spr:
	li a7, 110
	ecall
	ret

print:
	li a7, 111
	ecall
	ret

btn:
	li a7, 50
	ecall
	ret

btnp:
	li a7, 51
	ecall
	ret

putc:
	li a7, 52
	ecall
	ret

shutdown:
	li a7, 53
	ecall
	ret
