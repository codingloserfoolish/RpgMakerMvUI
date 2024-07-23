cmake_minimum_required(VERSION 3.10)

project(Manager)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)

find_package(Qt5 REQUIRED Widgets)

add_library(${PROJECT_NAME} STATIC
"PixmapNode.h"
"PixmapManager.h"
"PartPropertyManager.h"
 "PathManager.h"

"PixmapManager.cpp"
"PartPropertyManager.cpp"
 "PathManager.cpp")

target_link_libraries(${PROJECT_NAME} Qt5::Widgets)