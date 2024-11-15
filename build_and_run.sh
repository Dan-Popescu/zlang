#!/usr/bin/env bash


# Generate Make files
cmake ..

# Compile project
make

# Runs program with valgrind checks
if [ -f "./zlang" ]; then
    valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes -s ./zlang
else
    echo "Error : program file to run not provied."
    exit 1
fi
