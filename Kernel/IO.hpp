#pragma once
#include <stdint.h>
#include <stddef.h>

namespace Kernel::IO {
    inline uint8_t inb(uint16_t port) {
        uint8_t ret;
        asm volatile ( "inb %1, %0"
                : "=a"(ret)
                : "Nd"(port) );
        return ret;
    }
    inline void outb(uint16_t port, uint8_t value) {
        asm volatile ("outb %%al,%%dx": :"d" (port), "a" (value));
    }

    inline void delay(size_t microseconds) {
        for (size_t i = 0; i < microseconds; i++) {
            inb(0x80);
        }
    }
}
