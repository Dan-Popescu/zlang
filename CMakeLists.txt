cmake_minimum_required(VERSION 3.23)
project(zlang C)

set(CMAKE_C_STANDARD 23)

add_executable(zlang main.c op_s.h operators.h interpreters.c interpreters.h constants.h tokenizer.c tokenizer.h)
