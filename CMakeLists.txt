cmake_minimum_required(VERSION 3.8)
project(tree)

set(CMAKE_C_STANDARD 99)

set(SOURCE_FILES treeAPI.h treeAPI.c main.c input.txt)
add_executable(tree ${SOURCE_FILES})