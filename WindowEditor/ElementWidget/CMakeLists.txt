cmake_minimum_required(VERSION 3.10)

project(ElementWidget)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)

find_package(Qt5 REQUIRED Widgets)

add_library(${PROJECT_NAME} STATIC
"ElementEditLaunchWidget.h"
"ElementWidgetBase/ElementWidgetBase.h"
"LinesTextEditWidget/LinesTextEditWidget.h"
"BitmapEditWidget/BitmapEditWidget.h"
 "SpinBox/SpinBox.h" 


"ElementEditLaunchWidget.cpp"
"ElementWidgetBase/ElementWidgetBase.cpp"
"LinesTextEditWidget/LinesTextEditWidget.cpp"
"BitmapEditWidget/BitmapEditWidget.cpp"
"SpinBox/SpinBox.cpp"
)

target_link_libraries(${PROJECT_NAME} Qt5::Widgets)