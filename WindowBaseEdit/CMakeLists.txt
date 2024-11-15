cmake_minimum_required(VERSION 3.10)
set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_STANDARD 11)

set(COMPILE_IWINBASE FALSE)
set(COMPILE_MANAGER FALSE)

if(CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
set(OUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}"/../bin/release")
else()
set(OUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}"/../bin")
endif()

if(COMPILE_IWINBASE)
set(OUT_PATH_ELEMENT ${OUT_PATH})
elseif(COMPILE_MANAGER)
set(OUT_PATH_ELEMENT ${OUT_PATH})
else()
set(OUT_PATH_ELEMENT ${OUT_PATH}"/plugin")
endif()

if(COMPILE_IWINBASE OR COMPILE_MANAGER)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${OUT_PATH_ELEMENT}"/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${OUT_PATH_ELEMENT}"/lib")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OUT_PATH_ELEMENT}"/dll")
else()
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${OUT_PATH_ELEMENT}"/elementlib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${OUT_PATH_ELEMENT}"/elementlib")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OUT_PATH_ELEMENT}"/elementdll")
endif()

find_package(Qt5 COMPONENTS Core Widgets Xml)
if(NOT COMPILE_IWINBASE)
find_library(TOOLS tools PATHS ${OUT_PATH}"/lib")
find_library(MANAGER manager PATHS ${OUT_PATH}"/lib")
find_library(IBASE IWinBaseElement PATHS ${OUT_PATH}"/lib")
endif()

if(COMPILE_IWINBASE)

add_library(IWinBaseElement SHARED 
"WindowBaseElement/IWinBaseElement.cpp"
)
target_link_libraries(IWinBaseElement 
Qt5::Core
Qt5::Widgets
Qt5::Xml
)
target_compile_definitions(IWinBaseElement PRIVATE DLLIWINBASEELEMENTEXPORT)
target_include_directories(IWinBaseElement PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

elseif(COMPILE_MANAGER)

add_library(elementManager SHARED 
"ElementEditManager/ElementLoader.cpp"
"ElementEditManager/ElementsFactory.cpp"
)
target_link_libraries(elementManager 
Qt5::Core
Qt5::Widgets
Qt5::Xml
${IBASE}
)
target_compile_definitions(elementManager PRIVATE DLL_ELEMENT_MANAGER_EXPORT)
target_include_directories(elementManager PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
else()
add_library(ImageElement SHARED 
"WindowBaseElement/ImageElement.cpp"
)
target_link_libraries(ImageElement 
Qt5::Core
Qt5::Widgets
Qt5::Xml
${TOOLS}
${MANAGER}
${IBASE}
)
target_compile_definitions(ImageElement PRIVATE DLLIMAGEELEMENTEXPORT)
target_include_directories(ImageElement PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_library(TextElement SHARED 
"WindowBaseElement/TextElement.cpp"
)
target_link_libraries(TextElement 
Qt5::Core
Qt5::Widgets
Qt5::Xml
${TOOLS}
${MANAGER}
${IBASE}
)
target_compile_definitions(TextElement PRIVATE DLLTEXTELEMENTEXPORT)
target_include_directories(TextElement PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
endif()