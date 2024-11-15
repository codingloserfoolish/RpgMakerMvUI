cmake_minimum_required(VERSION 3.10)
project(RpgObjectDll)
set(CMAKE_AUTOMOC ON)
find_package(Qt5 COMPONENTS Core Widgets Xml)

set(COMPILE_WINDOW TRUE)

if(CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
set(OUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}"/../bin/release")
else()
set(OUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}"/../bin")
endif()

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${OUT_PATH}"/plugin/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${OUT_PATH}"/plugin/lib")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OUT_PATH}"/plugin/dll")
find_library(TOOLS_LIB tools PATHS ${OUT_PATH}"/lib")
find_library(MANAGER_LIB manager PATHS ${OUT_PATH}"/lib")
find_library(ITEM_LIB item PATHS ${OUT_PATH}"/lib")

if(COMPILE_WINDOW)
find_library(ELEMENT_MANAGER elementManager PATHS ${OUT_PATH}"/lib")
find_library(IBASE IWinBaseElement PATHS ${OUT_PATH}"/lib")
endif()

if(NOT COMPILE_WINDOW)
add_library(Sprite SHARED
"Sprite.cpp"
"SpriteEditor.cpp"
)
target_link_libraries(Sprite 
Qt5::Core 
Qt5::Widgets 
Qt5::Xml
${MANAGER_LIB}
${ITEM_LIB}
)
target_compile_definitions(Sprite PRIVATE DLL_SPRITE_EPORT)
target_include_directories(Sprite PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_library(Text SHARED
"Text.cpp"
"TextEditor.cpp"
)
target_link_libraries(Text 
Qt5::Core 
Qt5::Widgets 
Qt5::Xml
${TOOLS_LIB}
${MANAGER_LIB}
${ITEM_LIB}
)
target_compile_definitions(Text PRIVATE DLL_TEXT_EXPORT)
target_include_directories(Text PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
else()
add_library(WindowBase SHARED
"WindowBase.cpp"
"WindowBaseEditor.cpp"
"../WindowBaseEdit/ElementEditManager/ElementEditorManager.cpp"
"../WindowBaseEdit/Widget/ElementDisplayWidget.cpp"
"../WindowBaseEdit/Widget/Menu.cpp"
)
target_link_libraries(WindowBase 
Qt5::Core 
Qt5::Widgets 
Qt5::Xml
${MANAGER_LIB}
${ITEM_LIB}
${IBASE}
${ELEMENT_MANAGER}
)
target_compile_definitions(WindowBase PRIVATE DLL_WINDOWBASE_EXPORT)
target_include_directories(WindowBase PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
endif()