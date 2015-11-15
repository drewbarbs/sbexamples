include(ExternalProject)

set(EXTERN_PROJECT_PREFIX "${CMAKE_CURRENT_BINARY_DIR}/work")

ExternalProject_Add(
        sdl2
        PREFIX "${EXTERN_PROJECT_PREFIX}/sdl2"
        HG_REPOSITORY http://hg.libsdl.org/SDL/
        HG_TAG release-2.0.3
        CMAKE_ARGS "-DCMAKE_INSTALL_PREFIX:PATH=${CMAKE_CURRENT_BINARY_DIR}"
        UPDATE_COMMAND ""
)

set(GLEW_PREFIX_DIR "${EXTERN_PROJECT_PREFIX}/glew")
set(GENERATED_DIR "${EXTERN_PROJECT_PREFIX}/glew-generated")

file(MAKE_DIRECTORY "${GENERATED_DIR}")
file(WRITE "${GENERATED_DIR}/CMakeLists.txt"
        "cmake_minimum_required( VERSION 3.0 )\n"
        "add_subdirectory( build/cmake )\n"
)

ExternalProject_Add(
        glew
        PREFIX "${GLEW_PREFIX_DIR}"
        URL http://downloads.sourceforge.net/project/glew/glew/1.13.0/glew-1.13.0.tgz
        CMAKE_ARGS -DBUILD_UTILS:STRING=FALSE "-DCMAKE_INSTALL_PREFIX:PATH=${CMAKE_CURRENT_BINARY_DIR}"
        PATCH_COMMAND    "${CMAKE_COMMAND}" -E copy "${GENERATED_DIR}/CMakeLists.txt" "${GLEW_PREFIX_DIR}/src/glew"
        UPDATE_COMMAND ""
)

set(EXTERN_INCLUDE_DIR "${CMAKE_CURRENT_BINARY_DIR}/include" PARENT_SCOPE)
set(EXTERN_LIB_DIR "${CMAKE_CURRENT_BINARY_DIR}/lib" PARENT_SCOPE)
