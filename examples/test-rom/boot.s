.globl _start
.globl _init
.globl _update
.globl _draw
.globl pget
.globl pset
.globl cls
.globl clip
.globl rect
.globl rectfill
.globl line
.globl pal
.globl spr_data
.globl spr
.globl tile
.globl btn
.globl btnp
.globl putc
.globl shutdown
.globl push_offset
.globl pop_offset
.globl get_offset

.section .start

_start:
	la sp, __stack_top
	
	la t0, _gameloop
	csrw 0x7C2, t0
	
	jal ra, _init
	
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

pget:
	li a7, 101
	ecall
	ret

pset:
	li a7, 102
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

pal:
	li a7, 110
	ecall
	ret

spr_data:
	li a7, 111
	ecall
	ret

spr:
	li a7, 112
	ecall
	ret

tile:
	li a7, 113
	ecall
	ret

push_offset:
	li a7, 114
	ecall
	ret

pop_offset:
	li a7, 115
	ecall
	ret

get_offset:
	li a7, 116
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

