#include <kernel/Devices/PS2KeyboardDevice.hpp>
#include <kernel/Devices/SerialDevice.hpp>

namespace Kernel {

    bool PS2KeyboardDevice::initialize() {
        disable_devices();
        flush_output_buffer();
        set_config_byte();

        if (!perform_controller_self_test()) {
            g_serial_device->write_string("Failed to perform controller self test\n");
            return false;
        }

        if (m_is_dual_channel) {
            determine_dual_channel();
        }

        if (!perform_interface_test()) {
            g_serial_device->write_string("Failed to perform interface test\n");
            return false;
        }

        enable_devices();

        /*if (!reset_devices()) {
            g_serial_device->write_string("Failed to reset devices\n");
            return false;
        }*/

        return true;
    }

#define EMP 0x0
#define KEYBOARD_DATA_PORT 0x60

#define ON      1
#define OFF     0
#define LOWER   0
#define HIGHER  1
#define SPECIAL 1
#define COMMON  0

/* Special Keycodes */
#define BCKSPACE                0x08
#define ENTER                   0x0A

#define L_SHIFT_PRESS           0x2A
#define L_SHIFT_RELEASE         0xAA
#define R_SHIFT_PRESS           0x36
#define R_SHIFT_RELEASE         0xB6
#define CTRL_PRESS              0x1D
#define CTRL_RELEASE            0x9D
#define ALT_PRESS               0x38
#define ALT_RELEASE             0xB8
#define CAPS_PRESS              0x3A

#define F1                      0x3B
#define F2                      0x3C
#define F3                      0x3D

/* KEY_FLAGS */
    static uint8_t l_shift_flag = OFF;
    static uint8_t r_shift_flag = OFF;
    static uint8_t ctrl_flag = OFF;
    static uint8_t caps_flag = OFF;
    static uint8_t alt_flag = OFF;

#define SHIFT_FLAG              (l_shift_flag | r_shift_flag)


    #define SCANCODE_SET_SIZE 58
    uint8_t scancode_to_ascii[SCANCODE_SET_SIZE][2] = {     // two ascii char for each entry
            {EMP, EMP}, {EMP, EMP},
            {'1', '!'}, {'2', '@'},
            {'3', '#'}, {'4', '$'},
            {'5', '%'}, {'6', '^'},
            {'7', '&'}, {'8', '*'},
            {'9', '('}, {'0', ')'},     // A
            {'-', '_'}, {'=', '+'},
            {BCKSPACE, BCKSPACE}, {' ', ' '},
            {'q', 'Q'}, {'w', 'W'},      // 10, 11
            {'e', 'E'}, {'r', 'R'},
            {'t', 'T'}, {'y', 'Y'},
            {'u', 'U'}, {'i', 'I'},
            {'o', 'O'}, {'p', 'P'},
            {'[', '{'}, {']', '}'},      // 1A 1B
            {ENTER, ENTER}, {EMP, EMP},  // , Left Control
            {'a', 'A'}, {'s', 'S'},
            {'d', 'D'}, {'f', 'F'},     // 20, 21
            {'g', 'G'}, {'h', 'H'},
            {'j', 'J'}, {'k', 'K'},
            {'l', 'L'}, {';', ':'},     // 26, 27
            {'\'', '"'}, {'`', '~'},    // 28, 29
            {EMP, EMP}, {'\\', '|'},    // Left Shift, 2A, 2B
            {'z', 'Z'}, {'x', 'X'},
            {'c', 'C'}, {'v', 'V'},
            {'b', 'B'}, {'n', 'N'},     // 30, 31
            {'m', 'M'}, {',', '<'},     // 32, 33
            {'.', '>'}, {'/', '?'},     // 34, 35
            {EMP, EMP}, {EMP, EMP},     // Right Shift,
            {EMP, EMP}, {' ', ' '},
    };

    uint8_t PS2KeyboardDevice::read_data() {
        if (!prepare_for_input()) {
            return -1;
        }
        auto ret = IO::inb((uint16_t)PS2Port::Data);

        return scancode_to_ascii[ret][LOWER];
    }


    void PS2KeyboardDevice::disable_devices() {
        wait_then_write_command(PS2Command::DisableFirstPort);
        wait_then_write_command(PS2Command::DisableSecondPort);
    }

    bool PS2KeyboardDevice::flush_output_buffer() {
        for (int attempt = 0; attempt < 50; attempt++) {
            const auto status = IO::inb((uint16_t)PS2Port::Status);
            if (!(status & (uint8_t)StatusFlags::OutputBufferStatus)) {
                return true;
            }
            IO::inb((uint16_t)PS2Port::Data);
            IO::delay(100);
        }
        g_serial_device->write_string("Failed to flush output buffer\n");
        return false;
    }

    void PS2KeyboardDevice::set_config_byte() {
        wait_then_write_command(PS2Command::ReadConfigByte);
        auto config_byte = wait_then_read_data();
        // Clear bits 0, 1 and 6 to disable translation, disable IRQ1 and disable IRQ12
        config_byte &= ~((uint8_t)ConfigFlags::FirstPortInterrupt | (uint8_t)ConfigFlags::SecondPortInterrupt /*| (uint8_t)ConfigFlags::Translation*/);

        // If this is clear, we know it can't be a dual channel controller because the second ps/2 port is disabled
        m_is_dual_channel = (config_byte & (uint8_t) ConfigFlags::SecondPortClock) != 0;
        if (m_is_dual_channel) {
            g_serial_device->write_string("Dual channel PS/2 controller detected\n");
        } else {
            g_serial_device->write_string("Single channel PS/2 controller detected\n");
        }

        wait_then_write_command(PS2Command::WriteConfigByte);
        wait_then_write_data(config_byte);
    }

    bool PS2KeyboardDevice::perform_controller_self_test() {
        auto status = send_command(PS2Command::TestController);
        // Check if test passed
        if ((uint8_t)status != (uint8_t)PS2Response::ControllerTestPassed) {
            return false;
        }
        return true;
    }

    void PS2KeyboardDevice::determine_dual_channel() {
        wait_then_write_command(PS2Command::EnableSecondPort);
        auto config_byte = (uint8_t)send_command(PS2Command::ReadConfigByte);
        // If this is set it can't be a dual channel controller because the second ps/2 port is enabled
        m_is_dual_channel = (config_byte & (uint8_t) ConfigFlags::SecondPortClock) != 0;

        if (m_is_dual_channel) {
            send_command(PS2Command::DisableSecondPort);
        }
        // Enable IRQs for the ports that are usable
        if (m_first_port_available || m_second_port_available) {
            config_byte &= ~(uint8_t)ConfigFlags::FirstPortClock;
            config_byte &= ~(uint8_t)ConfigFlags::SecondPortClock;
            wait_then_write((uint16_t)PS2Port::Command, (uint8_t)PS2Command::WriteConfigByte);
            wait_then_write((uint16_t)PS2Port::Data, config_byte);
        }
    }

    bool PS2KeyboardDevice::perform_interface_test() {
        auto status = send_command(PS2Command::TestFirstPort);
        // Check if test passed
        m_first_port_available = status == PS2Response::FirstPortTestPassed;

        if (!m_first_port_available) {
            g_serial_device->write_string("First PS/2 port not available\n");
        }

        if (m_is_dual_channel) {
            status = send_command(PS2Command::TestSecondPort);

            g_serial_device->write_string("Second port test status: ");
            g_serial_device->write_hex((uint8_t)status);
            g_serial_device->write_string("\n");
            // Check if test passed
            m_second_port_available = status == PS2Response::SecondPortTestPassed;
            if (!m_second_port_available) {
                g_serial_device->write_string("Second PS/2 port not available\n");
            }
        }

        return m_first_port_available || m_second_port_available;
    }

    void PS2KeyboardDevice::enable_devices() {
        if (m_first_port_available) {
            wait_then_write_command(PS2Command::EnableFirstPort);
            g_serial_device->write_string("First PS/2 port enabled\n");
            // TODO: Disable first port clock?
        }
        flush_output_buffer();
        if (m_second_port_available) {
            wait_then_write_command(PS2Command::EnableSecondPort);
            g_serial_device->write_string("Second PS/2 port enabled\n");
            // TODO: Disable second port clock?
        }
        // TODO: enable this if using IRQs
        /*
        write_command(PS2Command::ReadConfigByte);
        auto config_byte = read_data();
        config_byte |= (uint8_t)ConfigFlags::FirstPortInterrupt | (uint8_t)ConfigFlags::SecondPortInterrupt;
        write_command(PS2Command::WriteConfigByte);
        write_data(config_byte);
         // Do same with second port if available
         */
    }

    bool PS2KeyboardDevice::reset_devices() {
        flush_output_buffer();
        auto status = send_command(PS2Command::ResetController);
        if (status != PS2Response::Acknowledge) {
            g_serial_device->write_string("Status not ack\n");
            g_serial_device->write_hex((uint8_t)status);
            g_serial_device->write_string("\n");
            return false;
        }
        status = (PS2Response)wait_then_read_data();
        if (status != PS2Response::Success) {
            g_serial_device->write_string("Status not success\n");
            g_serial_device->write_hex((uint8_t)status);
            g_serial_device->write_string("\n");
            return false;
        }
        return true;
    }

    bool PS2KeyboardDevice::prepare_for_input() {
        const uint8_t second_port_flag = 0;
        for (int i = 0; i < 1000; i++) {
            const auto status = read_status();
            // Check if output buffer is empty
            if (!(status & (uint8_t)StatusFlags::OutputBufferStatus)) {
                IO::delay(1000);
                continue;
            }
            if ((status & (uint8_t)StatusFlags::SecondPS2PortOutputBuffer) == second_port_flag) {
                return true;
            }
            IO::delay(1000);
        }
        //g_serial_device->write_string("PS/2 device not ready\n");
        return false;
    }

    bool PS2KeyboardDevice::prepare_for_output() {
        for (int i = 0; i < 250; i++) {
            const auto status = read_status();
            // Check if input buffer is empty
            if (!(status & (uint8_t)StatusFlags::InputBufferStatus)) {
                return true;
            }
            IO::delay(1000);
        }
        g_serial_device->write_string("PS/2 controller timed out\n");
        return false;
    }

    uint8_t PS2KeyboardDevice::wait_then_read_data() {
        prepare_for_input();
        return IO::inb((uint16_t)PS2Port::Data);
    }

    PS2Response PS2KeyboardDevice::send_command(PS2Command command) {
        return write_command(command);
    }
    PS2Response PS2KeyboardDevice::write_command(PS2Command command) {
        PS2Response response;
        int attempts = 0;
        do {
            prepare_for_output();
            IO::outb((uint16_t)PS2Port::Command, (uint8_t)command);

            response = (PS2Response)wait_then_read_data();
        } while (response == PS2Response::Resend && ++attempts < 250);
        if (attempts > 250) {
            g_serial_device->write_string("PS/2 controller failed to respond, gave up\n");
        }
        return response;
    }


    void PS2KeyboardDevice::wait_then_write_command(PS2Command command) {
        wait_then_write((uint16_t)PS2Port::Command, (uint8_t)command);
    }

    void PS2KeyboardDevice::wait_then_write_data(uint8_t data) {
        wait_then_write((uint16_t)PS2Port::Data, data);
    }
    void PS2KeyboardDevice::wait_then_write(uint16_t port, uint8_t data) {
        prepare_for_output();
        IO::outb(port, data);
    }
}