#!/bin/bash

mkdir -p isodir/boot/grub
cp voidos.bin isodir/boot/voidos.bin
cp ../configs/grub.cfg isodir/boot/grub/grub.cfg
grub-mkrescue -o voidos.iso isodir
rm -rf isodir
