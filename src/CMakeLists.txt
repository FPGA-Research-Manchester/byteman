cmake_minimum_required(VERSION 3.10)

project(byteman)


set(CMAKE_CXX_STANDARD 11)

add_executable(byteman byteman.cxx byteman.h version.h)

cmake -P IncrMinorVersion.cmake
