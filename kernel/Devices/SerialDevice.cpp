#include <kernel/Devices/SerialDevice.h>
#include <stddef.h>

namespace Kernel {

    static constexpr uint8_t DATA_REGISTER = 0;
    static constexpr uint8_t INTERRUPT_ENABLE_REGISTER = 1;
    static constexpr uint8_t FIFO_CONTROL_REGISTER = 2;
    static constexpr uint8_t LINE_CONTROL_REGISTER = 3;
    static constexpr uint8_t MODEM_CONTROL_REGISTER = 4;
    static constexpr uint8_t LINE_STATUS_REGISTER = 5;
    static constexpr uint8_t MODEM_STATUS_REGISTER = 6;
    static constexpr uint8_t SCRATCH_REGISTER = 7;



    void SerialDevice::set_interrupts(bool enabled){
        write_reg(INTERRUPT_ENABLE_REGISTER, enabled);
    }
    void SerialDevice::set_baud(BaudRate baud) {
        write_reg(LINE_CONTROL_REGISTER, 0x80);    // Enable DLAB (allow access to baud rate divisor)
        write_reg(0, (uint8_t)(baud) & 0xFFF);     // Least significant byte of divisor
        write_reg(1, (uint8_t)(baud) << 0x2);      // Most significant byte of divisor
        write_reg(LINE_CONTROL_REGISTER, 0x7f);    // Disable DLAB
    }

    void SerialDevice::set_line_control(DataBits data_bits, StopBits stop_bits, Parity parity) {
        // Bits 0-1: data bits
        // Bit 2: stop bit
        // Bits 3-5: parity
        write_reg(LINE_CONTROL_REGISTER, (uint8_t)data_bits | (uint8_t)stop_bits | (uint8_t)parity);    // Enable DLAB (allow access to baud rate divisor)
    }


    // https://wiki.osdev.org/Serial_Ports
    bool SerialDevice::initialize(COMPort port, BaudRate baud) {
        m_port = port;
        set_interrupts(false);
        set_baud(baud);
        set_line_control(DataBits::EIGHT, StopBits::ONE, Parity::NONE);
        //outb(m_port + 3, 0x03);    // 8 bits, no parity, one stop bit

        write_reg(FIFO_CONTROL_REGISTER, 0xC7);        // Enable FIFO, clear them, with 14-byte threshold
        write_reg(MODEM_CONTROL_REGISTER, 0x0B);       // IRQs enabled, RTS/DSR set
        write_reg(MODEM_CONTROL_REGISTER, 0x1E);       // Set in loopback mode, test the serial chip
        write_reg(DATA_REGISTER, 0xAE);                // Test serial chip (send byte 0xAE and check if serial returns same byte)

        // Check if serial is faulty (i.e: not same byte as sent)
        if (inb(port + DATA_REGISTER) != 0xAE) {
            return false;
        }

        // If serial is not faulty set it in normal operation mode
        // (not-loopback with IRQs enabled and OUT#1 and OUT#2 bits enabled)
        write_reg(MODEM_CONTROL_REGISTER, 0x0F);
        return true;
    }

    void SerialDevice::write(char a) {
        while (!is_transmit_empty());

        write_reg(DATA_REGISTER, a);
    }

    size_t strlen(const char* str)
    {
        size_t len = 0;
        while (str[len]) {
            len++;
        }
        return len;
    }

    void SerialDevice::write_string(const char *data) {
        for (size_t i = 0; i < strlen(data); ++i) {
            write(data[i]);
        }
    }

}