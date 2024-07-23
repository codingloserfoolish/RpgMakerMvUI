cmake_minimum_required(VERSION 3.10)

project(RPGMakerUI)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 11)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)

set(APP_ICON_RESOURCE_WINDOWS "${CMAKE_CURRENT_SOURCE_DIR}/RPGMakerUI.rc")

find_package(Qt5 REQUIRED Widgets)

add_executable(${PROJECT_NAME} main.cpp ${RPG_SOURCES} ${APP_ICON_RESOURCE_WINDOWS})

add_subdirectory(QtPropertyBrowser)
add_subdirectory(Manager)
add_subdirectory(RpgObject)
add_subdirectory(MyTransform)
add_subdirectory(Widget)

target_link_libraries(${PROJECT_NAME} 
qtpropertybrowser
Manager
RpgObject
MyTransform
Widget
)