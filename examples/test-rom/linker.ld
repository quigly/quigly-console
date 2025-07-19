ENTRY(_start)

MEMORY {
	ROM (rx) : ORIGIN = 0x20000000, LENGTH = 128M
	RAM (rw) : ORIGIN = 0x2000000, LENGTH = 32M
}

__stack_size = 1M;

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
		. += __stack_size;
		__stack_top = .;
		__bss_end = .;
	}
	
	/DISCARD/ : { *(.comment) *(.gnu*) *(.note*) *(.eh_frame*) }
}

__heap_start = __bss_end;
