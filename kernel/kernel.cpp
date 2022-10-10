#include <kernel/TTY/TTY.h>
#include <kernel/Devices/SerialDevice.h>
#include <kernel/Devices/PS2KeyboardDevice.h>
#include <sys/types.h>
#include <Shared/StringView.h>

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
}
}