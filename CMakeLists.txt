cmake_minimum_required(VERSION 3.10)
#next line has to be in the format ..."byteman VERSION X.Y"... for the autoversioning
project(byteman VERSION 0.2)

set(CMAKE_CXX_STANDARD 11)

add_executable(byteman src/byteman.cpp src/byteman.h)

