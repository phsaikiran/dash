cmake_minimum_required(VERSION 3.23)
project(dash C)

set(CMAKE_C_STANDARD 99)

include_directories(.)

add_executable(dash
        dash.c
        parsecommand.c
        parsecommand.h
)
