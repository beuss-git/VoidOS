#!/bin/bash
cd "$(dirname "$0")" && cd ..

qemu-system-i386.exe -kernel bin/voidos.bin
