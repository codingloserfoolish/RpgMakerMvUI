cmake_minimum_required(VERSION 3.10)
project(Dialog)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)

find_package(Qt5 REQUIRED Widgets)
add_library(${PROJECT_NAME} STATIC 
"IconSelectedDialog.h"
"VariableSelectedDialog.h" 
"ColorSelectedDialog.h"

"IconSelectedDialog.cpp"
"VariableSelectedDialog.cpp"
"ColorSelectedDialog.cpp")
target_link_libraries(${PROJECT_NAME} Qt5::Widgets)