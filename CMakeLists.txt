cmake_minimum_required(VERSION 3.1)
project(sbexamples)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_subdirectory(extern)
include_directories(BEFORE ${EXTERN_INCLUDE_DIRS})
link_directories(BEFORE ${EXTERN_LIB_DIR})

include_directories(lib)

function(add_example name)
    set(multiValueArgs SHADERS)
    cmake_parse_arguments(EXAMPLE "" "" "${multiValueArgs}" ${ARGN})
    set(SRCS ${EXAMPLE_UNPARSED_ARGUMENTS})
    list(LENGTH SRCS num_srcs)
    if(num_srcs LESS 1)
        message(FATAL_ERROR "add_example passed no source files for example ${name}")
    endif()
    add_executable("${name}" ${SRCS}
        "${CMAKE_SOURCE_DIR}/lib/util.cpp"
        "${CMAKE_SOURCE_DIR}/lib/util.h"
        "${CMAKE_SOURCE_DIR}/lib/gl3w.c")
    target_link_libraries("${name}" SDL2 GL ${CMAKE_DL_LIBS})
    add_dependencies("${name}" sdl2)

    list(LENGTH EXAMPLE_SHADERS num_shaders)
    if(num_shaders)
        foreach(SHADER IN LISTS EXAMPLE_SHADERS)
            if(NOT IS_ABSOLUTE "${SHADER}")
                string(CONCAT SHADER "${CMAKE_CURRENT_SOURCE_DIR}/" "${SHADER}")
            endif()
            list(APPEND SHADER_PATHS "${SHADER}")
        endforeach()
        add_custom_target("${name}-shaders" ALL
            "${CMAKE_COMMAND}" -E copy_if_different
            ${SHADER_PATHS}
            $<TARGET_FILE_DIR:${name}>
            VERBATIM)
    endif()
endfunction()

# A "hello world" example with vertices hardcoded in the vertex shader
add_subdirectory(simpletri)
add_subdirectory(x11simpletri) # A version of the above directly using glX

add_subdirectory(movingtri)
