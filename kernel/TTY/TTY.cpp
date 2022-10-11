#include "TTY.hpp"
#include <sys/types.h>
#include <Shared/Utils.hpp>

namespace Kernel {
    void TTY::initialize() {
        for (size_t y = 0; y < VGA_HEIGHT; ++y) {
            for (size_t x = 0; x < VGA_WIDTH; ++x) {
                const size_t index = y * VGA_WIDTH + x;
                m_buffer[index] = vga_entry(' ', m_color);
            }
        }
    }

    void TTY::handle_overflow() {
        // Move all rows one row up
        memcpy(m_buffer, &m_buffer[VGA_WIDTH], VGA_WIDTH * VGA_HEIGHT * sizeof(uint16_t));

        // Put empty row
        for (size_t x = 0; x < VGA_WIDTH; ++x) {
            const size_t index = VGA_HEIGHT * VGA_WIDTH + x;
            m_buffer[index] = vga_entry(' ', m_color);
        }
    }

    void TTY::handle_newline() {
        m_column = 0;
        if (++m_row > VGA_HEIGHT) {
            handle_overflow();
        }
    }

    void TTY::putchar(byte c) {
        //volatile uint64_t its = 10000000;
        //terminal_putchar((byte)sleep(its));
        //(void)sleep(its);
        if (c == '\n') {
            handle_newline();
            return;
        }
        putentryat(c, m_color, m_column, min<size_t>(m_row, VGA_HEIGHT));
        if (++m_column == VGA_WIDTH) {
            handle_newline();
        }
    }

    void TTY::write_char(char c) {
        putchar(c);
    }

    void TTY::write(const char *data, size_t size) {
        for (size_t i = 0; i < size; ++i) {
            putchar(data[i]);
        }
    }

    void TTY::write_string(const char *data) {
        write(data, strlen(data));
    }

    void TTY::write_rev(const char *buffer, size_t len) {
        while (len) {
            putchar(buffer[--len]);
        }
    }

    void TTY::write_num(uint32_t num) {
        size_t len = 0;
        char buf[MAX_DIGITS]{};
        do {
            const char digit = (char)(num % 10);
            buf[len++] = (char)(digit + '0');
            num /= 10;
        } while (num);

        write_rev(buf, len);
    }


}