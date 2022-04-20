cmake_minimum_required(VERSION 3.10)
#next line has to be in the format ..."byteman VERSION X.Y.Z"... for the autoversioning
project(byteman VERSION 0.0.44)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_AUTORCC ON)

add_executable(byteman src/byteman.cpp src/byteman.h Resources/icon.rc)

