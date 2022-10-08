#!/bin/bash

output_dir='bin'
kernel_dir='../kernel'

# Assemble boot object
i686-elf-as "${kernel_dir}"/boot.s -o "${output_dir}"/boot.o

# Compile kernel object
i686-elf-gcc -c "${kernel_dir}"/kernel.cpp -o "${output_dir}"/kernel.o -ffreestanding -O2 -Wall -Wextra -fno-exceptions -fno-rtti

# Link object files
i686-elf-gcc -T "${kernel_dir}"/linker.ld -o "${output_dir}"/myos.bin -ffreestanding -O2 -nostdlib "${output_dir}"/boot.o "${output_dir}"/kernel.o -lgcc

# Verify grub image
if grub-file --is-x86-multiboot "${output_dir}"/myos.bin; then
    echo multiboot confirmed
else
    echo the file is not multiboot
fi