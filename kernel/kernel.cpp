#include <stddef.h>
#include <stdint.h>
#include <test.h>
#include <stdlib.h>
#include <kernel/TTY/TTY.h>
#include <kernel/Devices/SerialDevice.h>

#if defined(__linux__)
#error "Not using a cross-compiler!"
#endif

#if !defined(__i386__)
#error "This needs to be compiled with a ix86-elf compiler!"
#endif

#if !defined(__voidos__)
#error "This needs a custom toolchain!"
#endif




size_t terminal_row;
size_t terminal_column;
uint8_t terminal_color;
uint16_t* terminal_buffer;



template <typename T>
T clamp(T value, T min, T max) 
{
    if (value < min) {
        return min;
    }
    if (value > max) {
        return max;
    }
    return value;
}

template <typename T>
T max(T a, T b) {
    return a > b ? a : b;
}



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





/*

*/
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
    tty.write_string("This is a test string!");
    serial.write_string("This is a test!");
    //terminal_initialize();
    //terminal_writestring(give_me_string());
    //terminal_writestring("Hello, kernel world!\nThis is a multiline message!\n This should appear on the next line!\nAnd this is a really really really long line that should overflow the columns and go on to a new line automatically or something");
    uint32_t i = 0;
    //while(true) {
        //write_serial_string("hellon\n");
        //terminal_putchar((char)(i % 127));
        //terminal_writenum(i);
        //terminal_putchar(' ');
        //i++;
    //}
}
}