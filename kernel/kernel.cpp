#include <kernel/TTY/TTY.h>
#include <kernel/Devices/SerialDevice.h>
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

extern "C" {
[[maybe_unused]] void kernel_main() {
    Kernel::SerialDevice serial;
    if (!serial.initialize(Kernel::COM1, Kernel::BaudRate::BPS_115200)) {
        return;
    }
    Kernel::TTY tty;
    tty.initialize();

    const StringView welcome_message {R"(
************************************************************
*                    Welcome to Void OS!                   *
************************************************************
)"};
    tty.write_string(welcome_message.data());
    serial.write_string(welcome_message.data());
}
}