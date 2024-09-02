cmake_minimum_required(VERSION 3.10)

project(WindowElement)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)

find_package(Qt5 REQUIRED Widgets Xml)

add_library(${PROJECT_NAME} STATIC
"WinElementBase.h"
"wbLinesText.h"
"wbBitmap.h"
"WinElementFactory.h" 

"WinElementBase.cpp"
"wbLinesText.cpp"
"wbBitmap.cpp"
"WinElementFactory.cpp")

target_link_libraries(${PROJECT_NAME} Qt5::Widgets Qt5::Xml)