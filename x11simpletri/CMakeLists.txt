add_executable(glxtri glxtri.cpp
        ${CMAKE_SOURCE_DIR}/lib/gl3w.c
        ${CMAKE_SOURCE_DIR}/lib/util.cpp
        ${CMAKE_SOURCE_DIR}/simpletri/simpletri.cpp
        )
target_link_libraries(glxtri Xrandr X11 GL ${CMAKE_DL_LIBS})
add_custom_command(TARGET glxtri PRE_BUILD
        COMMAND "${CMAKE_COMMAND}" -E copy "${CMAKE_SOURCE_DIR}/simpletri/vert.glsl" $<TARGET_FILE_DIR:glxtri>
        COMMAND "${CMAKE_COMMAND}" -E copy "${CMAKE_SOURCE_DIR}/simpletri/frag.glsl" $<TARGET_FILE_DIR:glxtri>)
