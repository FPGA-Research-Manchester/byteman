cmake_minimum_required(VERSION 3.10)

project(byteman
VERSION 0.0
)

set(CMAKE_CXX_STANDARD 11)

add_subdirectory(src)

add_executable(byteman src/byteman.cxx src/byteman.h)

