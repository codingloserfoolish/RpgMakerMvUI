cmake_minimum_required(VERSION 3.10)

project(Widget)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)

find_package(Qt5 REQUIRED Widgets Xml)

add_library(${PROJECT_NAME} STATIC
"GraphicsWindow.h"
"LineEditorEnterWidget.h"
"MainWindow.h"
"MenuExtend.h"
"PixmapSelectedWidget.h"
"FontLabel.h"
 "ObjectsTreeWidget.h"

"GraphicsWindow.cpp"
"LineEditorEnterWidget.cpp"
"MainWindow.cpp"
"MenuExtend.cpp"
"PixmapSelectedWidget.cpp"
"FontLabel.cpp"
"ObjectsTreeWidget.cpp")

target_link_libraries(${PROJECT_NAME} Qt5::Widgets Qt5::Xml)