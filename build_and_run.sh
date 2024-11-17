#!/usr/bin/env bash

# Generate Make files
cmake ..

# Compile project
make

# Runs program with valgrind checks
if [ -f "./zlang" ]; then
    if [ $# -eq 1 ]; then
        # Run zlang with the provided file as input
        valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes -s ./zlang "$1"
    else
        # Run zlang without arguments (REPL mode)
        valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes -s ./zlang
    fi
else
    echo "Error: Program file to run not provided."
    exit 1
fi
