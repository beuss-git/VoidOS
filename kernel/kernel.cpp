#include <stddef.h>
#include <stdint.h>
#include <test.h>
#include <stdlib.h>
#include <kernel/TTY/TTY.h>
#include <kernel/Devices/SerialDevice.h>
#include <sys/types.h>

#if defined(__linux__)
#error "Not using a cross-compiler!"
#endif

#if !defined(__i386__)
#error "This needs to be compiled with a ix86-elf compiler!"
#endif

#if !defined(__voidos__)
#error "This needs a custom toolchain!"
#endif

uint64_t sleep(uint64_t iterations) {
    uint64_t count = 0;
    for (uint64_t i = 0; i < iterations; i++) {
        if (i > iterations) {
            count = 0;
        }
        count++;
    }
    return count;
}

extern "C" {
[[maybe_unused]] void kernel_main()
{
    Kernel::SerialDevice serial;
    if (!serial.initialize(Kernel::COM1, Kernel::BaudRate::BPS_115200)) {
        return;
    }
    //write_serial_string("\033[1;31mbold red text\033[0m\n");
//for(;;)
    //asm("hlt");
    Kernel::TTY tty;
    tty.initialize();
    //tty.write_string("This is a test string!");
    //serial.write_string("This is a test!");
    memcpy(nullptr, nullptr, 0);

    serial.write_string("value at nullptr: ");

    *(int*)nullptr = 0xDEADBEEF;
    serial.write_hex(*(int*)nullptr);
    /*
    volatile int value = (int)nullptr;
    *(int*)value = 123;
    serial.write_num(*(int*)value);
     */
    //terminal_initialize();
    //terminal_writestring(give_me_string());
    //terminal_writestring("Hello, kernel world!\nThis is a multiline message!\n This should appear on the next line!\nAnd this is a really really really long line that should overflow the columns and go on to a new line automatically or something");
    //uint32_t i = 0;
    //while(true) {
        //write_seral_string("hellon\n");
        //terminal_putchar((char)(i % 127));
        //terminal_writenum(i);
        //terminal_putchar(' ');
        //i++;
    //}
}
}