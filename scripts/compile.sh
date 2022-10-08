#!/bin/bash

output_dir='bin'
kernel_dir='../kernel'

# Copy library files (primarily headers) to sysroot dir that was specified to gcc and binutils when building them
cp -RT ../Userland/Libraries/LibC ../sysroot/usr/include

# Assemble boot object
i686-voidos-as "${kernel_dir}"/boot.s -o "${output_dir}"/boot.o

# Compile kernel object
i686-voidos-gcc -c "${kernel_dir}"/kernel.cpp -o "${output_dir}"/kernel.o -ffreestanding -O2 -Wall -Wextra -fno-exceptions -fno-rtti

# Link object files
i686-voidos-gcc -T "${kernel_dir}"/linker.ld -o "${output_dir}"/voidos.bin -ffreestanding -O2 -nostdlib "${output_dir}"/boot.o "${output_dir}"/kernel.o -lgcc

# Verify grub image
if grub-file --is-x86-multiboot "${output_dir}"/voidos.bin; then
    echo multiboot confirmed
else
    echo the file is not multiboot
fi