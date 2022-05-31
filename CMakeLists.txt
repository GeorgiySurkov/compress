cmake_minimum_required(VERSION 3.21)

project(boolcalc)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(
        compress
        compress.cpp
        algorithms.h
        algorithms.cpp
        algorithms/Huffman.h
        algorithms/Huffman.cpp
        algorithms/LZW.h
        algorithms/LZW.cpp
        lib/avl_map.h
        lib/cstring.h
        lib/cstring.cpp
        lib/functional.h
        lib/utility.h
        lib/vector.h
)