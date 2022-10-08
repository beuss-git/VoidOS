/*
Set the _start symbol as the entry point for the bootloader to look for
*/
ENTRY(_start)


/* Tell where the varios sections of the object files will be put in the final kernel image. */
SECTIONS
{
    /* Begin putting sections at 1MiB. This is a conventional place for kernels to be loaded at by the bootloader */
    . = 1M;

    /* First put the multiboot header, needs to be within the first 8KiB. */
    .text BLOCK(4k) : ALIGN(4k)
    {
        *(.multiboot)
        *(.text)
    }

    /* Read-only data */
    .data BLOCK(4k) : ALIGN (4k) 
    {
        *(.rodata)
    }

    /* Read-write data (initialized) */
    .data BLOCK(4k) : ALIGN (4k)
    {
        *(.data)
    }

    /* Read-write data (uninitialized) and stack */
    .data BLOCK(4k) : ALIGN (4k)
    {
        *(COMMON)
        *(.bss)
    }

    /* Compiler may produce other sections, and by default put them in a segment with the same name. 
       Add here as needed :)
    */
}