add_example(simpletri simpletri.cpp main.cpp)
add_custom_command(TARGET simpletri PRE_BUILD
        COMMAND "${CMAKE_COMMAND}" -E copy "${CMAKE_CURRENT_SOURCE_DIR}/vert.glsl" $<TARGET_FILE_DIR:simpletri>
        COMMAND "${CMAKE_COMMAND}" -E copy "${CMAKE_CURRENT_SOURCE_DIR}/frag.glsl" $<TARGET_FILE_DIR:simpletri>)

