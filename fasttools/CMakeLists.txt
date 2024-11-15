cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 11)

if(CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}"/../bin/release/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}"/../bin/release/lib")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}"/../bin/release/dll")
else()
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}"/../bin/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}"/../bin/lib")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}"/../bin/dll")
endif()

find_package(Qt5 COMPONENTS Core Gui)
add_library(tools SHARED
"fasttools.cpp"
"tools.cpp"
)
target_link_libraries(tools Qt5::Core Qt5::Gui)
target_compile_definitions(tools PRIVATE DLLEXPORT)

target_include_directories(tools PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})