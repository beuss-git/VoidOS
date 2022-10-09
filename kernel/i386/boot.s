/* Declare constants for the multiboot header.*/
.set ALIGN,     1<<0             /* align loaded modules on page boundaries */
.set MEMINFO,   1<<1             /* provide memory map */
.set FLAGS,     ALIGN | MEMINFO  /* multiboot 'flag' field */
.set MAGIC,     0x1BADB002       /* 'magic number' lets bootloader find header */
.set CHECKSUM,  -(MAGIC + FLAGS) /* checksum of above, to prove we are multiboot */

/*
MULTIBOOT:

- Mark program as a kernel
- Add the magic multiboot values to our multiboot section to be found by bootloader
*/
.section .multiboot
.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM

/*
BSS:
- 'Allocate' 16 KiB stack space
- Provides stack pointer (bottom and top)
- Must be 16-byte aligned
*/
.section .bss
.align 16
stack_bottom:
.skip 16384 # 16 KiB
stack_top:

/*
TEXT: 
- Actual program code, in this case the kernel entry point
*/
.section .text
.global _start
.type _start, @function
_start:
    /*
    Current state:
    - Bootloader has loaded us into 32-bit protected mode on x86.
    - Interrupts disabled
    - Paging disabled
    - Kernel has full CPU control
    - Kernel can only use hardware features
    - Processor state is as defined in the multiboot standard
    - Processor not fully set up yet
    - Floating point instructions disabled
    - Instruction set extensions not initialized
    - No exceptions set up by runtime yet
    */

    /*
    Set up stack by pointing esp to the top of the stack
    */
    mov $stack_top, %esp

    /*
    Enter high-level kernel. 
    ABI requires that the stack pointer is 16-byte aligned at the time of the call instruction. (which afterwards pushes the return pointer of size 4 bytes)
    The stack was originally 16-byte aligned and we pushed a multiple of 16 bytes to the stack since (0 bytes so far), so alignment is preserved and the call is well defined.
    */
    call kernel_main

    /*
    If the system has nothing more to do, put the computer into an infinite loop.

    1. Disable interrupts with cli (clears interrupt enable in eflags)
    2. Wait for the next interrupt to arrive with hlt (halt instruction)
       Since they are disabled, this will lock up the computer
    3. Jump to the hlt instruction if it ever wakes up due to a non-maskable interrupt occurring or due to system management mode
    */
    cli
1:  hlt
    jmp 1b


/*
Set size of the _start symbol to current location '.' minus it's start.
Useful for debugging or when implementing call tracing
*/
.size _start, . - _start
