ENABLE_LANGUAGE(ASM)

set(SHARED_SOURCES
        ${CMAKE_SOURCE_DIR}/Shared/Utils.cpp
)

set(KERNEL_BIN "voidos.bin")
set(KERNEL_ISO ${PROJECT_NAME}.iso)

# Kernel binary
add_executable(${KERNEL_BIN}
    ${arch}/boot.s
    Kernel.cpp
    Devices/PS2KeyboardDevice.cpp
    Devices/SerialDevice.cpp
    TTY/TTY.cpp
    ${SHARED_SOURCES}
)

target_include_directories(${KERNEL_BIN} PUBLIC ${CMAKE_SOURCE_DIR})
target_compile_features(${KERNEL_BIN} PUBLIC cxx_std_20 c_std_11)
target_link_libraries(${KERNEL_BIN} PRIVATE libc)
#target_compile_options(${KERNEL_BIN} PRIVATE -ffreestanding -O0 -Wall -Wextra -fno-exceptions -fno-rtti)
set_target_properties(${KERNEL_BIN} PROPERTIES LINK_FLAGS "-T ${CMAKE_CURRENT_SOURCE_DIR}/${arch}/linker.ld -ffreestanding -O2 -nostdlib -lgcc")
add_custom_command(TARGET ${KERNEL_BIN} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_SOURCE_DIR}/bin
        COMMAND ${CMAKE_COMMAND} -E copy ${KERNEL_BIN} ${CMAKE_SOURCE_DIR}/bin/
        COMMENT "Copying binary"
        )

# Generate ISO file
add_custom_target(${KERNEL_ISO}
        COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/Scripts/build-image.sh
        COMMENT "Generating kernel iso file"
        BYPRODUCTS ${KERNEL_ISO}
        )

add_dependencies(${KERNEL_ISO} ${KERNEL_BIN})
add_custom_command(TARGET ${KERNEL_ISO} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_SOURCE_DIR}/bin
        COMMAND ${CMAKE_COMMAND} -E copy ${KERNEL_ISO} ${CMAKE_SOURCE_DIR}/bin/
        )
