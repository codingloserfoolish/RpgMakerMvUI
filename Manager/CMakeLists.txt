cmake_minimum_required(VERSION 3.10)

project(Manager)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)

find_package(Qt5 REQUIRED Widgets Xml)

add_library(${PROJECT_NAME} STATIC
"PixmapNode.h"
"PixmapManager.h"
"PartPropertyManager.h"
"PathManager.h"
"JsImageReserveContainer.h" 
"ConfigureInfoContainer.h"
"WinFontCalculate.h"
"RpgObjectEditorManager.h"

"PixmapManager.cpp"
"PartPropertyManager.cpp"
"PathManager.cpp"
"JsImageReserveContainer.cpp"
"ConfigureInfoContainer.cpp"
"WinFontCalculate.cpp"
"RpgObjectEditorManager.cpp")

target_link_libraries(${PROJECT_NAME} Qt5::Widgets Qt5::Xml)