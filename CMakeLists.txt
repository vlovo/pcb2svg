
 
cmake_minimum_required(VERSION 3.15)

 
project(pcb2svg
DESCRIPTION ""
HOMEPAGE_URL "" 
VERSION 1.0 
LANGUAGES CXX
)

set(SRC  main.cpp  )
set(HDR ${CMAKE_SOURCE_DIR}/svg/simple_svg_1.0.0.hpp)
set(DOCS Readme.md)

add_executable(${PROJECT_NAME} ${SRC} ${HDR})
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/svg)

set_target_properties(${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)