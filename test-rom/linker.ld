ENTRY(_start)

MEMORY {
	ROM (rx) : ORIGIN = 0x2000000, LENGTH = 32M
	RAM (rw) : ORIGIN = 0x100000, LENGTH = 4M
}

SECTIONS
{
    .text : {
		KEEP(*(.start))
		*(.text*)
    } > ROM

    .rodata : {
		*(.rodata*)
	} > ROM

	.data : {
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

    /DISCARD/ : {
		*(.eh_frame)
    }
}
