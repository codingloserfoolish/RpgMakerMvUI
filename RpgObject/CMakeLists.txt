cmake_minimum_required(VERSION 3.10)

project(RpgObject)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)

find_package(Qt5 REQUIRED Widgets Xml)

add_library(${PROJECT_NAME} STATIC
"GameObject.h"
"RpgScene.h"
"RpgSprite.h"
"RpgText.h"
"RpgWindowBase.h"
"RpgObjectFactory.h"

"GameObject.cpp"
"RpgScene.cpp"
"RpgSprite.cpp"
"RpgText.cpp"
"RpgWindowBase.cpp"
"RpgObjectFactory.cpp"
 )


target_link_libraries(${PROJECT_NAME} Qt5::Widgets Qt5::Xml)