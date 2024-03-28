cmake_minimum_required(VERSION 3.5)

project(fixed_point)

add_executable(fixed_point main.cc)

set(CMAKE_CXX_STANDARD 17)

set(GCC_COVERAGE_COMPILE_FLAGS "-Wall -Wextra -std=c++17")
add_definitions(${GCC_COVERAGE_COMPILE_FLAGS})