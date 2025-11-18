cmake_minimum_required(VERSION 4.0)
project(polynomials C)

set(CMAKE_C_STANDARD 11)
add_compile_options(
    -std=c23
    -pedantic
    -Wall
    -Wextra
    -Wformat-security
    -Wduplicated-cond
    -Wfloat-equal
    -Wshadow
    -Wconversion
    -Wjump-misses-init
    -Wlogical-not-parentheses
    -Wnull-dereference
    -Wvla
    -Werror
    -fstack-protector-strong
    -fsanitize=undefined
    -fno-sanitize-recover
    -g
    -fno-omit-frame-pointer
    -O1
)
add_executable(polynomials main.c)
