cmake_minimum_required(VERSION 2.8.12)
project(sbexamples)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y")

add_subdirectory(extern)
include_directories(BEFORE ${EXTERN_INCLUDE_DIRS})
link_directories(BEFORE ${EXTERN_LIB_DIR})

include_directories(lib)

function(add_example name sources)
    add_executable("${name}" "${sources}" "${CMAKE_SOURCE_DIR}/lib/util.cpp" "${CMAKE_SOURCE_DIR}/lib/gl3w.c")
    target_link_libraries("${name}" SDL2 GL ${CMAKE_DL_LIBS})
    add_dependencies("${name}" sdl2)
endfunction()

add_subdirectory(simpletri)