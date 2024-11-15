#如何编译滑倒最下面
#
cmake_minimum_required(VERSION 3.10)
project(RpgMakerMvUI)
set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_STANDARD 11)

set(APP_ICON_RESOURCE_WINDOWS ${CMAKE_CURRENT_SOURCE_DIR}"/resources.rc")

if(CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
set(EXECUTABLE_PATH ${CMAKE_CURRENT_SOURCE_DIR}"/bin/release")
set(LIB_FIND_PATH ${CMAKE_CURRENT_SOURCE_DIR}"/bin/release/lib")
else()
set(EXECUTABLE_PATH ${CMAKE_CURRENT_SOURCE_DIR}"/bin")
set(LIB_FIND_PATH ${CMAKE_CURRENT_SOURCE_DIR}"/bin/lib")
endif()

find_package(Qt5 COMPONENTS Core Widgets Xml)

set(EXECUTABLE_OUTPUT_PATH ${EXECUTABLE_PATH})

find_library(TOOLS tools PATHS ${LIB_FIND_PATH})
find_library(MANAGER manager PATHS ${LIB_FIND_PATH})
find_library(ITEM item PATHS ${LIB_FIND_PATH})
find_library(ELEMENT_MANAGER elementManager PATHS ${LIB_FIND_PATH})

aux_source_directory("scene" SCENE_SRC)
aux_source_directory("view" VIEW_SRC)
aux_source_directory("widget" WIDGET_SRC)
set(MANAGER_SRC 
"manager/EditorLaunch.cpp" 
"manager/ObjectsFactory.cpp"
"manager/ObjectLoader.cpp")
aux_source_directory("xmlParser" XMLPARSER_SRC)
aux_source_directory("javascriptParser" JSPARSER_SRC)

add_executable(${PROJECT_NAME} WIN32
"main.cpp"
${SCENE_SRC}
${VIEW_SRC}
${WIDGET_SRC}
${MANAGER_SRC}
${XMLPARSER_SRC}
${JSPARSER_SRC}
${APP_ICON_RESOURCE_WINDOWS}
)

target_link_libraries(${PROJECT_NAME} 
Qt5::Core
Qt5::Widgets
Qt5::Xml
${TOOLS}
${ITEM}
${MANAGER}
${ELEMENT_MANAGER}
)

#编译顺序fasttools->item->
#WindowBaseEdit(进入先COMPILE_IWINBASE=TRUE COMPILE_MANAGER=FALSE  ->
#COMPILE_IWINBASE=FALSE COMPILE_MANAGER=TRUE->COMPILE_IWINBASE=FALSE COMPILE_MANAGER=FALSE)->
#manager->
#rpgobject (进入COMPILE_WINDOW=TRUE->COMPILE_WINDOW=FALSE)
#add_subdirectory(fasttools)
#add_subdirectory(item)
#add_subdirectory(WindowBaseEdit)
#add_subdirectory(manager)
#add_subdirectory(rpgobject)