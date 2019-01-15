cmake_minimum_required(VERSION 3.12)
project(Rendu3D)

set(CMAKE_CXX_STANDARD 14)

include_directories(src)

add_executable(Rendu3D
        src/main.cpp
        src/tgaimage.cpp
        src/tgaimage.h src/geometry.h)
