cmake_minimum_required(VERSION 3.10)
project(BoringFunction)
set(CMAKE_CXX_STANDARD 11)
add_library(${PROJECT_NAME} STATIC
"BoringFunction.h"
"BoringFunction.cpp"
)