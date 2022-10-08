#!/bin/bash

/bin/bash ./compile.sh
qemu-system-i386.exe -kernel bin/voidos.bin


# Not this only works if the iso image is on the windows disk.. and not in a wsl path
# See: https://www.reddit.com/r/osdev/comments/t71lf1/qemu_failing_to_load_iso_alignment_issue/
#/bin/bash ./compile
#/bin/bash ./build-image
#qemu-system-i386.exe -cdrom voidos.iso