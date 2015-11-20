cmake_minimum_required(VERSION 3.3)
project(sbexamples)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y")

add_subdirectory(extern)
include_directories(BEFORE "${EXTERN_INCLUDE_DIR}")
link_directories(BEFORE "${EXTERN_LIB_DIR}")

include_directories(lib)

function(add_example name sources)
    add_executable("${name}" "${sources}" "${CMAKE_SOURCE_DIR}/lib/util.cpp")
    target_link_libraries("${name}" GLEW SDL2 GL)
    add_dependencies("${name}" sdl2 glew)
endfunction()

add_subdirectory(simpletri)