#!/bin/bash


# Generate Make files
cmake ..

# Compile project
make

# Rung program with valgrind checks
if [ -f "./zlang" ]; then
    valgrind --leak-check=full --track-origins=yes -s ./zlang
else
    echo "Error : program file to run not provied."
    exit 1
fi
