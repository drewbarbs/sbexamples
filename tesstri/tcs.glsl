#version 450 core

// Number of output vertices per patch
layout (vertices = 3) out;

void main(void) {
  if (gl_InvocationID == 0) {
    gl_TessLevelInner[0] = 5.0;
    gl_TessLevelOuter[0] = 5.0;
    gl_TessLevelOuter[1] = 5.0;
    gl_TessLevelOuter[2] = 5.0;
  }

  // This copying of input to output is "optional" according to
  // https://www.khronos.org/opengl/wiki/Tessellation_Control_Shader#Built-in_outputs
  // But if you don't write the vertices out then they won't be
  // drawn/considered for tesselation
  gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
}
