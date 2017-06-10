add_example(tesstri tesstri.cpp main.cpp
  SHADERS vert.glsl frag.glsl tcs.glsl tes.glsl)

add_example(tessgstri tesstri.cpp main.cpp
  SHADERS geom.glsl)

add_dependencies(tessgstri tesstri)
set_property(TARGET tessgstri APPEND PROPERTY COMPILE_DEFINITIONS "WITH_GEOM")
set_target_properties(tessgstri PROPERTIES
  COMPILE_DEFINITIONS "${CDEFS}")
