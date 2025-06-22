SECTIONS
{
    . = 0x100000;

    .text : {
		KEEP(*(.start))
		*(.text)
		*(.text*)
        *(.rodata*)
        *(.rodata* .rodata.*)
        *(.sdata*)
        *(.sbss*)
    }

    .data : {
        *(.data)
    }

    .bss : {
        *(.bss)
    }

    /DISCARD/ : {
		*(.eh_frame)
    }
}
