cmake_minimum_required(VERSION 3.5)

project(fixed_point)

add_executable(fixed_point main.cc fixed_point.cc)

SET(GCC_COVERAGE_COMPILE_FLAGS "-Wall -std=c++17")
add_definitions(${GCC_COVERAGE_COMPILE_FLAGS})