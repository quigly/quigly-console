ENTRY(_start)

MEMORY {
	ROM (rx) : ORIGIN = 0x2000000, LENGTH = 32M
	RAM (rw) : ORIGIN = 0x100000, LENGTH = 4M
}

SECTIONS
{
    .text : ALIGN(4) {
		KEEP(*(.start))
		*(.text*)
		*(.srodata*)
		*(.nodata*)
    } > ROM

    .rodata : ALIGN(4) {
		*(.rodata*)
	} > ROM

	.data : ALIGN(4) {
		__data_load = LOADADDR(.data);
		__data_start = .;
		*(.data*)
		__data_end = .;
	} > RAM AT > ROM

	.bss : ALIGN(4) {
		__bss_start = .;
		*(.bss*)
		*(COMMON)
		__bss_end = .;
	}
	
	/DISCARD/ : { *(.comment) *(.gnu*) *(.note*) *(.eh_frame*) }
}

_heap_start = __bss_end;
_stack_top = 0x500000 - 16;
