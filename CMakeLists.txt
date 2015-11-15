cmake_minimum_required(VERSION 3.3)
project(sbexamples)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y")

add_subdirectory(extern)
include_directories(BEFORE "${EXTERN_INCLUDE_DIR}")
link_directories(BEFORE "${EXTERN_LIB_DIR}")

set(SOURCE_FILES main.cpp)
add_executable(sbexamples ${SOURCE_FILES})
target_link_libraries(sbexamples SDL2)
add_dependencies(sbexamples glew sdl2)

