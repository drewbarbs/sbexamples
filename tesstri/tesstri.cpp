#include <cmath>
#include <memory>

#include "tesstri.h"
#include "util.h"

const double pi = std::acos(-1);

void tesstri::do_render(double current_time) {
  // Clear the buffer
  GLfloat clearcolor[] = {1.0f, 1.0f, 1.0f, 1.0f};
  glClearBufferfv(GL_COLOR, 0, clearcolor);

  glUseProgram(prog.get());
  glDrawArrays(GL_PATCHES, 0, 3);
  check_glerror();
}

tesstri::tesstri(): prog(glCreateProgram(), glDeleteProgram) {
  // Load vertex and frag shader sources, initialize
  // prog member
  auto vert_src = sb::read_file("vert.glsl");
  auto vert_shader = sb::scope_handle<GLuint>(glCreateShader(GL_VERTEX_SHADER),
                                              glDeleteShader);
  sb::shader_src_and_compile(vert_shader.get(), vert_src);
  glAttachShader(prog.get(), vert_shader.get());

  auto tcs_src = sb::read_file("tcs.glsl");
  auto tcs_shader = sb::scope_handle<GLuint>(glCreateShader(GL_TESS_CONTROL_SHADER),
                                             glDeleteShader);
  sb::shader_src_and_compile(tcs_shader.get(), tcs_src);
  glAttachShader(prog.get(), tcs_shader.get());

  auto tes_src = sb::read_file("tes.glsl");
  auto tes_shader = sb::scope_handle<GLuint>(glCreateShader(GL_TESS_EVALUATION_SHADER),
                                             glDeleteShader);
  sb::shader_src_and_compile(tes_shader.get(), tes_src);
  glAttachShader(prog.get(), tes_shader.get());

#ifdef WITH_GEOM
  auto geom_src = sb::read_file("geom.glsl");
  auto geom_shader = sb::scope_handle<GLuint>(glCreateShader(GL_GEOMETRY_SHADER),
    glDeleteShader);

  sb::shader_src_and_compile(geom_shader.get(), geom_src);
  glAttachShader(prog.get(), geom_shader.get());
#endif

  auto frag_src = sb::read_file("frag.glsl");
  auto frag_shader = sb::scope_handle<GLuint>(glCreateShader(GL_FRAGMENT_SHADER),
                                              glDeleteShader);
  sb::shader_src_and_compile(frag_shader.get(), frag_src);
  glAttachShader(prog.get(), frag_shader.get());

  glLinkProgram(prog.get());
  check_glerror();

  // Create a vertex array object and
  GLuint vaoi;
  glCreateVertexArrays(1, &vaoi);
  check_glerror();
  vao = sb::scope_handle<GLuint>(
      vaoi, [](GLuint vaoi) { glDeleteVertexArrays(1, &vaoi); });
  glBindVertexArray(vao.get());

#ifdef WITH_GEOM
  // Make the point size large
  glPointSize(5.0);
#else
  // Draw in wireframe, rather than solid color, to clearly see the
  // tessellated triangle
   glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
#endif // WITH_GEOM

}

tesstri::~tesstri() {
}
