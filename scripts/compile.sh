#!/bin/bash

cmake -S . -B build
# If we also want to build the iso image
#cmake --build build -t voidos.iso
cmake --build build