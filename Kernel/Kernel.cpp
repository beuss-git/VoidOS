#include <Kernel/TTY/TTY.hpp>
#include <Kernel/Devices/SerialDevice.hpp>
#include <Kernel/Devices/PS2KeyboardDevice.hpp>
#include <sys/types.h>
#include <Shared/StringView.hpp>

#if defined(__linux__)
#error "Not using a cross-compiler!"
#endif

#if !defined(__i386__)
#error "This needs to be compiled with a ix86-elf compiler!"
#endif

#if !defined(__voidos__)
#error "This needs a custom toolchain!"
#endif

extern "C" {
[[maybe_unused]] void kernel_main() {
    Kernel::SerialDevice serial;
    Kernel::g_serial_device = &serial;
    if (!serial.initialize(Kernel::COM1, Kernel::BaudRate::BPS_115200)) {
        return;
    }
    Kernel::TTY tty;
    tty.initialize();

    const StringView welcome_message {R"(
*************************************************************
*                    Welcome to Void OS!                    *
*************************************************************
)"};
    tty.set_color(Kernel::VGAColor::VGA_COLOR_LIGHT_BLUE);
    tty.write_string(welcome_message.data());
    serial.set_color(Shared::Bash::Color::LightBlue);
    serial.write_string(welcome_message.data());

    serial.set_color(Shared::Bash::Color::Red);
    serial.write_string("This message should be red\n");

    serial.set_color(Shared::Bash::Color::Green);
    serial.write_string("This message should be green\n");

    serial.set_color(Shared::Bash::Color::Magenta);
    serial.write_string("This message should be magenta\n");

    serial.set_color(Shared::Bash::Color::Default);
    Kernel::PS2KeyboardDevice keyboard;
    keyboard.initialize();

    while (true) {
        const auto kb_char = keyboard.read_data();
        if (kb_char == (uint8_t)-1 || kb_char == 0) {
            continue;
        }

        serial.write_char((char)kb_char);
        tty.write_char((char)kb_char);
    }
}
}