cmake_minimum_required(VERSION 3.10)
#next line has to be in the format ..."byteman VERSION X.Y.Z"... for the autoversioning
project(byteman VERSION 0.0.17)

set(CMAKE_CXX_STANDARD 11)

add_executable(byteman src/byteman.cpp src/byteman.h Resources/icon.rs)

