#pragma once
#include <stdint.h>

namespace Kernel {
    enum COMPort : uint16_t {
        COM1 = 0x3F8,
        COM2 = 0x2F8,
        COM3 = 0x3E8,
        COM4 = 0x2E8,
        COM5 = 0x5F8,
        COM6 = 0x4F8,
        COM7 = 0x5E8,
        COM8 = 0x4E8,
    };
    enum class BaudRate : uint32_t {
        BPS_50 = 2304,
        BPS_110 = 1047,
        BPS_220 = 524,
        BPS_300 = 384,
        BPS_600 = 192,
        BPS_1200 = 96,
        BPS_2400 = 48,
        BPS_4800 = 24,
        BPS_9600 = 12,
        BPS_19200 = 6,
        BPS_38400 = 3,
        BPS_57600 = 2,
        BPS_115200 = 1,
    };



    enum class DataBits : uint8_t {
        FIVE = 0b0 << 0,
        SIX = 0b1 << 0,
        SEVEN = 0b10 << 0,
        EIGHT = 0b11 << 0,
    };
    enum class StopBits : uint8_t {
        ONE = 0b0 << 2,
        TWO = 0b1 << 2,
    };

    enum class Parity : uint8_t {
        NONE = 0b0 << 3,
        ODD = 0b1 << 3,
        EVEN = 0b11 << 3,
        MARK = 0b101 << 3,
        SPACE = 0b111 << 3,
    };

    enum class ModemControl : uint8_t {
        DATA_TERMINAL_READY = 1 << 0,
        REQUEST_TO_SEND = 1 << 1,
        OUT_1 = 1 << 2,
        OUT_2 = 1 << 3,
        LOOPBACK = 1 << 4,
    };

    class SerialDevice {
        public:
        SerialDevice() = default;

        bool initialize(COMPort port, BaudRate rate);

        void write(char a);

        void write_string(const char* data);

    private:
        void set_interrupts(bool enabled);
        void set_baud(BaudRate rate);
        void set_line_control(DataBits data_bits, StopBits stop_bits, Parity parity);
        //void set_fifo_control()

        uint8_t inb(uint16_t port) {
            uint8_t ret;
            asm volatile ( "inb %1, %0"
                    : "=a"(ret)
                    : "Nd"(port) );
            return ret;
        }
        void outb(uint16_t port, uint8_t value) {
            asm volatile ("outb %%al,%%dx": :"d" (port), "a" (value));
        }
        bool is_transmit_empty() {
            return inb(m_port + 5) & 0x20;
        }

        void write_reg(uint8_t reg, uint8_t value) {
            outb(m_port + reg, value);
        }

        uint8_t read_reg(uint8_t reg) {
            return inb(m_port + reg);
        }

    private:
        COMPort m_port = COM1;
    };
}