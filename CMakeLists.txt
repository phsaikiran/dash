cmake_minimum_required(VERSION 3.23)
project(5348_project_1 C)

set(CMAKE_C_STANDARD 11)

add_executable(5348_project_1
        dash.c
        parsecommand.c
        tok.c parsecommand.h)
