#pragma once
#include <stdint.h>
#include <x86gprintrin.h>

namespace Kernel {
    // TODO: move this
    using byte = unsigned char;

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
    class TTY {
    public:
        TTY();
        ~TTY();

        void write(const char* data, size_t size);
        void write_string(const char* data);
        void terminal_writenum(uint32_t num);
        void set_color(VGAColor color) { m_color = color; }
    private:
        void terminal_write_rev(const char* buffer, size_t len);

        void handle_overflow();

        void handle_newline();

        void putchar(byte c);

        void putentryat(byte c, uint8_t color, size_t x, size_t y) {
            const size_t index = y * VGA_WIDTH + x;
            m_buffer[index] = vga_entry(c, color);
        }
        uint8_t vga_entry_color(VGAColor fg, VGAColor bg) {
            return fg | bg << 4;
        }

        uint16_t vga_entry(byte uc, uint8_t color) {
            return static_cast<uint16_t>(uc) | static_cast<uint16_t>(color) << 8;
        }
    private:
        static constexpr size_t VGA_WIDTH = 80;
        static constexpr size_t VGA_HEIGHT = 25;
        static constexpr size_t MAX_DIGITS = 255;

        size_t m_row{0};
        size_t m_column{0};

        uint8_t m_color{vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK)};
        uint16_t* m_buffer{(uint16_t*)0xB8000};
    };


}