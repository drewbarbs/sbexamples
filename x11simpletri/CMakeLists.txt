add_example(glxtri glxtri.cpp "${CMAKE_SOURCE_DIR}/simpletri/simpletri.cpp"
  SHADERS "${CMAKE_SOURCE_DIR}/simpletri/vert.glsl" "${CMAKE_SOURCE_DIR}/simpletri/frag.glsl")
target_link_libraries(glxtri Xrandr X11 GL ${CMAKE_DL_LIBS})
