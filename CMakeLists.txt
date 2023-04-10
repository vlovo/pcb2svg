
 
cmake_minimum_required(VERSION 3.15)

set(ProjectName  pcb2svg)

project(${ProjectName})

set(SRC  main.cpp)
set(HDR ${CMAKE_SOURCE_DIR}/svg/simple_svg_1.0.0.hpp)

add_executable(${ProjectName} ${SRC} ${HDR})
target_include_directories(${ProjectName} PRIVATE ${CMAKE_SOURCE_DIR}/svg)

set_target_properties(${ProjectName} PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)