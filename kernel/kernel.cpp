#include <stddef.h>
#include <stdint.h>
#include <test.h>
#include <stdlib.h>
#include <kernel/TTY/TTY.h>

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




uint8_t inb(uint16_t port)
{
    uint8_t ret;
    asm volatile ( "inb %1, %0"
                   : "=a"(ret)
                   : "Nd"(port) );
    return ret;
}

void outb(uint16_t port, uint8_t value)
{
    asm volatile ("outb %%al,%%dx": :"d" (port), "a" (value));
}
static constexpr auto PORT = 0x3f8; // COM1
bool init_serial()
{
    outb(PORT + 1, 0x00);    // Disable all interrupts
    outb(PORT + 3, 0x80);    // Enable DLAB (set baud rate divisor)
    outb(PORT + 0, 0x03);    // Set divisor to 3 (lo byte) 38400 baud
    outb(PORT + 1, 0x00);    //                  (hi byte)
    outb(PORT + 3, 0x03);    // 8 bits, no parity, one stop bit
    outb(PORT + 2, 0xC7);    // Enable FIFO, clear them, with 14-byte threshold
    outb(PORT + 4, 0x0B);    // IRQs enabled, RTS/DSR set
    outb(PORT + 4, 0x1E);    // Set in loopback mode, test the serial chip
    outb(PORT + 0, 0xAE);    // Test serial chip (send byte 0xAE and check if serial returns same byte)

    // Check if serial is faulty (i.e: not same byte as sent)
    if(inb(PORT + 0) != 0xAE) {
        return false;
    }

    // If serial is not faulty set it in normal operation mode
    // (not-loopback with IRQs enabled and OUT#1 and OUT#2 bits enabled)
    outb(PORT + 4, 0x0F);
    return true;
}

bool is_transmit_empty()
{
    return inb(PORT + 5) & 0x20;
}

/*
void write_serial(char a)
{
    while (!is_transmit_empty());

    outb(PORT, a);
}

void write_serial_string(const char* data)
{
    for (size_t i = 0; i < strlen(data); ++i) {
        write_serial(data[i]);
    }
}

*/
extern "C" {
[[maybe_unused]] void kernel_main()
{
    if (!init_serial()) {
        return;
    }
    //write_serial_string("\033[1;31mbold red text\033[0m\n");
//for(;;)
    //asm("hlt");
    Kernel::TTY tty;
    tty.write_string("This is a test string!");
    //terminal_initialize();
    //terminal_writestring(give_me_string());
    //terminal_writestring("Hello, kernel world!\nThis is a multiline message!\n This should appear on the next line!\nAnd this is a really really really long line that should overflow the columns and go on to a new line automatically or something");
    uint32_t i = 0;
    while(true) {
        //write_serial_string("hellon\n");
        //terminal_putchar((char)(i % 127));
        //terminal_writenum(i);
        //terminal_putchar(' ');
        i++;
    }
}
}