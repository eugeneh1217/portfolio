cmake_minimum_required(VERSION 3.6.0)

project(c_dynamic_array)

add_executable(
    demo
    src/main.c
)

target_include_directories(demo PUBLIC include/)
