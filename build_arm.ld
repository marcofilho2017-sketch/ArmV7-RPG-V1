ENTRY(_start)
MEMORY {
	ROM(rx) : ORIGIN = 0, LENGTH = 1M
	RAM(wx) : org = 0x00100000, len = 40M
}

/* Define output sections */
SECTIONS
{
    . = 0x00000000;

    .vectors : {
        vector_table = .;
        *(.vectors)
    }

    .text : {
        *(.text*)
        *(.rodata*)
    }

    . = ALIGN(4);

    .data : {
        *(.data*)
    }

    .bss : {
        __bss_start__ = .;
        *(.bss*)
        *(COMMON)
        __bss_end__ = .;
    }
}