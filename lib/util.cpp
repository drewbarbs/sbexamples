#include "util.h"

#include <fstream>
#include <sstream>

std::string sb::read_file(const std::string &fname) {
    std::ifstream in;
    in.exceptions(std::ifstream::badbit | std::ifstream::failbit);
    in.open(fname, std::ifstream::in);

    std::stringstream buf;
    buf << in.rdbuf();
    return buf.str();
}

void sb::shader_src(const GLuint shader, const std::string &src) {
  const char *srcp = src.c_str();
  glShaderSource(shader, 1, &srcp, nullptr);
  check_glerror();
}

void sb::_check_glerror(const char *file, int line) {
  for (auto err = glGetError(); err != GL_NO_ERROR; err = glGetError()) {
    switch(err) {
    case GL_INVALID_ENUM:
      throw std::runtime_error(std::string("GL_INVALID_ENUM at ") + file +
                               std::string(":") + std::to_string(line));
    case GL_INVALID_VALUE:
      throw std::runtime_error(std::string("GL_INVALID_VALUE at ") + file +
                               std::string(":") + std::to_string(line));
    case GL_INVALID_OPERATION:
      throw std::runtime_error(std::string("GL_INVALID_OPERATION at ") + file +
                               std::string(":") + std::to_string(line));
    case GL_INVALID_FRAMEBUFFER_OPERATION:
      throw std::runtime_error(
          std::string("GL_INVALID_FRAMEBUFFER_OPERATION at ") + file +
          std::string(":") + std::to_string(line));
    case GL_OUT_OF_MEMORY:
      throw std::runtime_error(std::string("GL_OUT_OF_MEMORY at ") + file +
                               std::string(":") + std::to_string(line));
    default:
      throw std::runtime_error(std::string("Unrecognized error ") +
                               std::to_string(err) + " at " + file +
                               std::string(":") + std::to_string(line));
    }
  }
}
