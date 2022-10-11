#pragma once
#include <stdint.h>
#include <Kernel/IO.hpp>

namespace Kernel {
    // https://wiki.osdev.org/%228042%22_PS/2_Controller
    enum class PS2Port : uint8_t {
        Data = 0x60,
        Status = 0x64,
        Command = 0x64
    };

    enum class PS2Command : uint8_t {
        ReadConfigByte = 0x20,
        WriteConfigByte = 0x60,
        DisableFirstPort = 0xAD,
        EnableFirstPort = 0xAE,
        DisableSecondPort = 0xA7,
        EnableSecondPort = 0xA8,
        TestFirstPort = 0xAB,
        TestSecondPort = 0xA9,
        TestController = 0xAA,
        DiagnosticDump = 0xAC,
        DisableSecondPortIRQ = 0xA3,
        EnableSecondPortIRQ = 0xA4,
        WriteOutputPort = 0xD4,
        DisableFirstPortIRQ = 0xAD,
        EnableFirstPortIRQ = 0xAE,
        ReadInputPort = 0xC0,
        ReadOutputPort = 0xD0,
        ResetController = 0xFF,
    };

    enum class StatusFlags : uint8_t {
        OutputBufferStatus = 1 << 0,
        InputBufferStatus = 1 << 1,
        SystemFlag = 1 << 2,
        CommandData = 1 << 3,
        Unk1 = 1 << 4,
        SecondPS2PortOutputBuffer = 1 << 5,
        TimeoutError = 1 << 6,
        ParityError = 1 << 7
    };

    enum class ConfigFlags : uint8_t {
        FirstPortInterrupt = 1 << 0,
        SecondPortInterrupt = 1 << 1,
        SystemFlag = 1 << 2,
        Zero = 1 << 3,
        FirstPortClock = 1 << 4,
        SecondPortClock = 1 << 5,
        Translation = 1 << 6
    };

    enum class PS2Response : uint8_t{
        ControllerTestPassed = 0x55,
        ControllerTestFailed = 0xFC,
        FirstPortTestPassed = 0x00,
        FirstPortTestFailed = 0x01,
        SecondPortTestPassed = 0x00,
        SecondPortTestFailed = 0x01,
        Success = 0xFA,
        Acknowledge = 0xFA,
        Resend = 0xFE
    };



    // Supports the 8042 PS/2 controller
    class PS2KeyboardDevice {
    public:
        PS2KeyboardDevice() = default;

        bool initialize();

        uint8_t read_data();

    private:
        /*
         * Part of setup
         */
        void disable_devices();
        bool flush_output_buffer();
        void set_config_byte();
        bool perform_controller_self_test();
        void determine_dual_channel();
        bool perform_interface_test();
        void enable_devices();
        bool reset_devices();

        bool prepare_for_input();
        bool prepare_for_output();

        uint8_t wait_then_read_data();

        PS2Response send_command(PS2Command command);
        PS2Response write_command(PS2Command command);
        void wait_then_write_command(PS2Command command);
        void wait_then_write_data(uint8_t data);
        void wait_then_write(uint16_t port, uint8_t data);

        uint8_t read_status() const {
            return IO::inb((uint16_t)PS2Port::Status);
        }
    private:
        bool m_is_dual_channel {true};

        bool m_first_port_available {false};
        bool m_second_port_available {false};
    };
}