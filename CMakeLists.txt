cmake_minimum_required(VERSION 4.0)
project(polynomials C)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)

add_compile_options(
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
