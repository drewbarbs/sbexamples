cmake_minimum_required(VERSION 3.1)
project(sbexamples)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(extern)
include_directories(BEFORE ${EXTERN_INCLUDE_DIRS})
link_directories(BEFORE ${EXTERN_LIB_DIR})

include_directories(lib)

function(add_example name)
    list(LENGTH ARGN num_sources)
    if(num_sources LESS 1)
        message(FATAL_ERROR "add_example passed no source files for example ${name}")
    endif()
    add_executable("${name}" ${ARGN} "${CMAKE_SOURCE_DIR}/lib/util.cpp" "${CMAKE_SOURCE_DIR}/lib/gl3w.c")
    target_link_libraries("${name}" SDL2 GL ${CMAKE_DL_LIBS})
    add_dependencies("${name}" sdl2)
endfunction()

add_subdirectory(simpletri)
add_subdirectory(x11simpletri)