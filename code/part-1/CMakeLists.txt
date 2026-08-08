cmake_minimum_required(VERSION 3.16)
project(part1_number_theory LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(rot13_example examples/rot13/main.cpp)
add_executable(modular_arithmetic_example examples/modular-arithmetic/main.cpp)
add_executable(part1_tests tests/part1_tests.cpp)

enable_testing()
add_test(NAME part1_tests COMMAND part1_tests)
