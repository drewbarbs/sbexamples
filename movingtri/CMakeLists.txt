add_example(movingtri movingtri.cpp main.cpp)
add_custom_command(TARGET movingtri POST_BUILD
        COMMAND "${CMAKE_COMMAND}" -E copy "${CMAKE_CURRENT_SOURCE_DIR}/vert.glsl" $<TARGET_FILE_DIR:movingtri>
        COMMAND "${CMAKE_COMMAND}" -E copy "${CMAKE_CURRENT_SOURCE_DIR}/frag.glsl" $<TARGET_FILE_DIR:movingtri>)
