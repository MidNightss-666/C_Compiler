cmake_minimum_required(VERSION 3.22)
project(C_Compiler)
set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++")
set(CMAKE_CXX_STANDARD 14)

add_executable(Part1 main.cpp)

add_executable(Part2 ../Part2/main.cpp)

add_executable(Part3 ../Part3/main.cpp)

add_executable(Part4 ../Part4/main.cpp)

add_executable(Part5 ../Part5/main.cpp)