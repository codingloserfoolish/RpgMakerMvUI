cmake_minimum_required(VERSION 3.10)

project(qtpropertybrowser)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)

find_package(Qt5 REQUIRED Widgets)

add_library(${PROJECT_NAME} STATIC
    qtpropertybrowser.cpp
    qtpropertymanager.cpp
    qteditorfactory.cpp
    qtvariantproperty.cpp
    qttreepropertybrowser.cpp
    qtbuttonpropertybrowser.cpp
    qtgroupboxpropertybrowser.cpp
    qtpropertybrowserutils.cpp

    qtpropertybrowser.h
    qtpropertymanager.h
    qteditorfactory.h
    qtvariantproperty.h
    qttreepropertybrowser.h
    qtbuttonpropertybrowser.h
    qtgroupboxpropertybrowser.h
    qtpropertybrowserutils_p.h

    qtpropertybrowser.qrc
 )

target_link_libraries(${PROJECT_NAME} Qt5::Widgets)