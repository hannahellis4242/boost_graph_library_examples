#!/bin/bash
set -e
clang-format -style=file main.cpp -i
make
./main
