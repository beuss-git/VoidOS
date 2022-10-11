cmake_minimum_required(VERSION 3.15)
project(voidos)

set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR i686)

ENABLE_LANGUAGE(ASM)

set(TOOLS $ENV{HOME}/opt/cross)
set(CMAKE_CXX_COMPILER ${TOOLS}/bin/i686-voidos-g++)
set(CMAKE_C_COMPILER ${TOOLS}/bin/i686-voidos-gcc)
set(CMAKE_ASM_COMPILER ${TOOLS}/bin/i686-voidos-as)

#set(CMAKE_CXX_STANDARD 20)
#set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_FLAGS "-ffreestanding -O0 -Wall -Wextra -fno-exceptions -fno-rtti")
set(CMAKE_C_FLAGS "-ffreestanding -O0 -Wall -Wextra")

set(KERNEL_BIN "voidos.bin")
set(KERNEL_ISO ${PROJECT_NAME}.iso)


#include_directories(Userland/Libraries/LibC)
add_subdirectory(Userland/Libraries/LibC)

add_library(testlib Userland/testlib.c)
target_compile_features(testlib PUBLIC cxx_std_20 c_std_11)

set(arch i386)


set(SHARED_SOURCES
    Shared/Utils.cpp
)

# Kernel binary
add_executable(${KERNEL_BIN} 
    kernel/${arch}/boot.s 
    kernel/kernel.cpp
    kernel/Devices/PS2KeyboardDevice.cpp
    kernel/Devices/SerialDevice.cpp
    kernel/TTY/TTY.cpp
    ${SHARED_SOURCES}
)
target_include_directories(${KERNEL_BIN} PUBLIC ${CMAKE_SOURCE_DIR})
target_compile_features(${KERNEL_BIN} PUBLIC cxx_std_20 c_std_11)
target_link_libraries(${KERNEL_BIN} PRIVATE testlib libc)
#target_compile_options(${KERNEL_BIN} PRIVATE -ffreestanding -O0 -Wall -Wextra -fno-exceptions -fno-rtti)
set_target_properties(${KERNEL_BIN} PROPERTIES LINK_FLAGS "-T ${CMAKE_SOURCE_DIR}/kernel/${arch}/linker.ld -ffreestanding -O2 -nostdlib -lgcc")
add_custom_command(TARGET ${KERNEL_BIN} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_SOURCE_DIR}/bin
    COMMAND ${CMAKE_COMMAND} -E copy ${KERNEL_BIN} ${CMAKE_SOURCE_DIR}/bin/
    COMMENT "Copying binary"
)

# Generate ISO file
add_custom_target(${KERNEL_ISO}
    COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/scripts/build-image.sh
    COMMENT "Generating kernel iso file"
    BYPRODUCTS ${KERNEL_ISO}
)

add_dependencies(${KERNEL_ISO} ${KERNEL_BIN})
#add_dependencies(voidos.iso voidos.bin)
add_custom_command(TARGET ${KERNEL_ISO} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_SOURCE_DIR}/bin
    COMMAND ${CMAKE_COMMAND} -E copy ${KERNEL_ISO} ${CMAKE_SOURCE_DIR}/bin/
)