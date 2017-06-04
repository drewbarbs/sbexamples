#include <cmath>
#include <memory>

#include "movingtri.h"
#include "util.h"

const double pi = std::acos(-1);

void movingtri::do_render(double current_time) {
  // Clear the buffer
  GLfloat clearcolor[] = {1.0f, 1.0f, 1.0f, 1.0f};
  glClearBufferfv(GL_COLOR, 0, clearcolor);

  glUseProgram(prog.get());
  GLfloat offset[] = {0.5f * static_cast<float> (std::cos(current_time)),
                      0.5f * static_cast<float>(std::sin(current_time)),
                      0.f, 0.f};
  glVertexAttrib4fv(0, offset);
  glDrawArrays(GL_TRIANGLES, 0, 3);
}

movingtri::movingtri(): prog(glCreateProgram(), glDeleteProgram) {
  // Load vertex and frag shader sources, initialize
  // prog member
  auto vert_src = sb::read_file("vert.glsl");
  auto vert_shader = sb::scope_handle<GLuint>(glCreateShader(GL_VERTEX_SHADER),
                                              glDeleteShader);
  sb::shader_src_and_compile(vert_shader.get(), vert_src);
  glAttachShader(prog.get(), vert_shader.get());

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
}

movingtri::~movingtri() {
}
