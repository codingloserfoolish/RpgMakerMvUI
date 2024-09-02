cmake_minimum_required(VERSION 3.10)

project(RpgObjectEditor)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 11)

find_package(Qt5 REQUIRED Widgets Xml)

add_library(${PROJECT_NAME} STATIC
"RpgObjectEditorLaunch.h" 
"RpgObjectEditorBase.h"
"RpgTextEditor.h"
"RpgWindowCommandEditor.h"

"RpgObjectEditorLaunch.cpp"
"RpgObjectEditorBase.cpp"
"RpgTextEditor.cpp"
"RpgWindowCommandEditor.cpp")

target_link_libraries(${PROJECT_NAME} Qt5::Widgets Qt5::Xml)