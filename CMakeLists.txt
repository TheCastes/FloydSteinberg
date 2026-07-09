cmake_minimum_required(VERSION 4.2)
project(FloydSteinberg)

set(CMAKE_CXX_STANDARD 20)

add_executable(FloydSteinberg main.cpp
        Image.h
        Dithering.h)
