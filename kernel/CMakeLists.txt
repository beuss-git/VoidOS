
# TODO: allow it to be set
set(KERNEL_ARCH i386)

set(KERNEL_SOURCES
    kernel.cpp
)


if(${KERNEL_ARCH} STREQUAL "i386")
    set(KERNEL_SOURCES
        ${KERNEL_SOURCES}
        i386/boot.s
    )
endif()


add_executable(Kernel ${KERNEL_SOURCES})