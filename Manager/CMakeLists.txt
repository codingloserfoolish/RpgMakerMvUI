cmake_minimum_required(VERSION 3.10)
project(manager)

set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_STANDARD 11)

find_package(Qt5 COMPONENTS Core Gui)

if(CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}"/../bin/release/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}"/../bin/release/lib")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}"/../bin/release/dll")
else()
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}"/../bin/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}"/../bin/lib")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}"/../bin/dll")
endif()

add_library(manager SHARED 
"ConfigInfo.cpp"
"ImageManager.cpp")

target_link_libraries(manager Qt5::Core Qt5::Gui)

target_compile_definitions(manager PRIVATE DLL_MANAGER_EXPORT)

target_include_directories(manager PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})