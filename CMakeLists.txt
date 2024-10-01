cmake_minimum_required(VERSION 3.23)
project(zlang C)

set(CMAKE_C_STANDARD 23)

add_executable(zlang
        main.c
        op_s.h
        operators.h
        interpreter.c
        interpreter.h
        constants.h
        lexer.c lexer.h)
