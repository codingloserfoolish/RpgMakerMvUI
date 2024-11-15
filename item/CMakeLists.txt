cmake_minimum_required(VERSION 3.10)
project(item)
set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_STANDARD 11)

find_package(Qt5 COMPONENTS Core Widgets Xml)

if(CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}"/../bin/release/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}"/../bin/release/lib")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}"/../bin/release/dll")
find_library(TOOLS tools PATHS ${CMAKE_CURRENT_SOURCE_DIR}"/../bin/release/lib")
else()
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}"/../bin/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}"/../bin/lib")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}"/../bin/dll")
find_library(TOOLS tools PATHS ${CMAKE_CURRENT_SOURCE_DIR}"/../bin/lib")
endif()

add_library(item SHARED
"GraphicsItem.cpp"
"ObjectEditor.cpp"
)

target_link_libraries(item 
Qt5::Core 
Qt5::Widgets 
Qt5::Xml
${TOOLS})

target_compile_definitions(item PRIVATE DLLITEMEXPORT)

target_include_directories(item PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})