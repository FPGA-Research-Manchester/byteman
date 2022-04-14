cmake_minimum_required(VERSION 3.11)

project(byteman
VERSION 3.11
)

set(CMAKE_CXX_STANDARD 11)

add_executable(byteman src/byteman.cpp src/byteman.h)

