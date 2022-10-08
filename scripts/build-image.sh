#!/bin/bash

output_dir='bin'

mkdir -p isodir/boot/grub
cp ${output_dir}/myos.bin isodir/boot/myos.bin
cp ../configs/grub.cfg isodir/boot/grub/grub.cfg
grub-mkrescue -o myos.iso isodir
