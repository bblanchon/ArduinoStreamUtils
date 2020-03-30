#!/bin/sh -eux

mkdir build
cd build
cmake --version
cmake ..
cmake --build .
ctest --output-on-failure .
