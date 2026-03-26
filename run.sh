#!/bin/bash
# COL106 Long Assignment 2 - SocialNet Simulator
# Compilation and execution script

# Stop if any command fails
set -e

# Compile
g++ -std=c++17 -O2 -Wall -o Assignment_2 Assignment_2.cpp

# Run
./Assignment_2
