#!/bin/bash

cd "$(dirname "$0")" && cd ..

scripts/compile.sh

# Verify grub image
if grub-file --is-x86-multiboot bin/voidos.bin; then
    echo multiboot confirmed
else
    echo the file is not multiboot
fi

qemu-system-i386.exe -kernel bin/voidos.bin


# Not this only works if the iso image is on the windows disk.. and not in a wsl path
# See: https://www.reddit.com/r/osdev/comments/t71lf1/qemu_failing_to_load_iso_alignment_issue/
#/bin/bash ./compile
#/bin/bash ./build-image
#qemu-system-i386.exe -cdrom voidos.iso