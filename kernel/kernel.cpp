#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#if defined(__linux__)
#error "Not using a cross-compiler!"
#endif

#if !defined(__i386__)
#error "This needs to be compiled with a ix86-elf compiler!"
#endif

enum VGAColor : uint8_t {
	VGA_COLOR_BLACK = 0,
	VGA_COLOR_BLUE = 1,
	VGA_COLOR_GREEN = 2,
	VGA_COLOR_CYAN = 3,
	VGA_COLOR_RED = 4,
	VGA_COLOR_MAGENTA = 5,
	VGA_COLOR_BROWN = 6,
	VGA_COLOR_LIGHT_GREY = 7,
	VGA_COLOR_DARK_GREY = 8,
	VGA_COLOR_LIGHT_BLUE = 9,
	VGA_COLOR_LIGHT_GREEN = 10,
	VGA_COLOR_LIGHT_CYAN = 11,
	VGA_COLOR_LIGHT_RED = 12,
	VGA_COLOR_LIGHT_MAGENTA = 13,
	VGA_COLOR_LIGHT_BROWN = 14,
	VGA_COLOR_WHITE = 15,
};

uint8_t vga_entry_color(VGAColor fg, VGAColor bg) 
{
    return fg | bg << 4;
}

using byte = unsigned char;
uint16_t vga_entry(byte uc, uint8_t color) 
{
    return static_cast<uint16_t>(uc) | static_cast<uint16_t>(color) << 8;
}

size_t strlen(const char* str) 
{
    size_t len = 0;
    while (str[len]) {
        len++;
    }
    return len;
}

static constexpr size_t VGA_WIDTH = 80;
static constexpr size_t VGA_HEIGHT = 25;

size_t terminal_row;
size_t terminal_column;
uint8_t terminal_color;
uint16_t* terminal_buffer;

void terminal_initialize() 
{
    terminal_row = 0;
    terminal_column = 0;
    terminal_color = vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
    terminal_buffer = (uint16_t*)0xB8000;
    for (size_t y = 0; y < VGA_HEIGHT; ++y) {
        for (size_t x = 0; x < VGA_WIDTH; ++x) {
            const size_t index = y * VGA_WIDTH + x;
            terminal_buffer[index] = vga_entry(' ', terminal_color);
        }
    }
}

void terminal_setcolor(uint8_t color) 
{
    terminal_color = color;
}

void terminal_putentryat(byte c, uint8_t color, size_t x, size_t y)
{
    const size_t index = y * VGA_WIDTH + x;
    terminal_buffer[index] = vga_entry(c, color);
}

void memcpy(void* dst, void* src, size_t num) 
{
    char* dest = (char*)dst;
    const char* source = (const char*)src;
    while (num) {
        *(dest++) = *(source++);
        num--;
    }
}

void handle_overflow() 
{
    // Move all rows one row up
    for (size_t row = 0; row < VGA_HEIGHT; ++row) {
        memcpy(&terminal_buffer[row * VGA_WIDTH],
               &terminal_buffer[(row + 1) * VGA_WIDTH],
               VGA_WIDTH * sizeof(uint16_t));
    }

    // Put empty row
    for (size_t x = 0; x < VGA_WIDTH; ++x) {
        const size_t index = VGA_HEIGHT * VGA_WIDTH + x;
        terminal_buffer[index] = vga_entry(' ', terminal_color);
    }
}

void handle_newline() {
    terminal_column = 0;
    if (++terminal_row > VGA_HEIGHT)
    {
        handle_overflow();
    }
}
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

template <typename T>
T min(T a, T b) {
    return a < b ? a : b;
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

void terminal_putchar(byte c) 
{

    volatile uint64_t its = 100000;
        //terminal_putchar((byte)sleep(its));
    (void)sleep(its);
    if (c == '\n') {
        handle_newline();
        return;
    }
    terminal_putentryat(c, terminal_color, terminal_column, min<size_t>(terminal_row, VGA_HEIGHT));
    if (++terminal_column == VGA_WIDTH) {
        handle_newline();
    }
}

void terminal_write(const char* data, size_t size) 
{
    for (size_t i = 0; i < size; ++i) {
        terminal_putchar(data[i]);
    }
}

void terminal_writestring(const char* data)
{
    terminal_write(data, strlen(data));
}

extern "C" {
    void kernel_main()
    {
        terminal_initialize();
        terminal_writestring("Hello, kernel world!\nThis is a multiline message!\n This should appear on the next line!\nAnd this is a really really really long line that should overflow the columns and go on to a new line automatically or something");
        int i = 0;
        while(true) {
            //terminal_writestring("AAAAAAAAAAAABBBBBB");
            terminal_putchar((char)(i % 127));
            i++;
        }
    }
}