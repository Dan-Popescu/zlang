cmake_minimum_required(VERSION 3.23)
project(zlang C)

set(CMAKE_C_STANDARD 23)

add_executable(zlang
        main.c
        interpreter.c
        interpreter.h
        constants.h
        lexer.c lexer.h abstract_syntax_tree.c abstract_syntax_tree.h parser.c parser.h)
