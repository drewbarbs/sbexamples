#include "util.h"

#include <vector>
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

void sb::shader_src_and_compile(const GLuint shader, const std::string &src) {
  const char *srcp = src.c_str();
  glShaderSource(shader, 1, &srcp, nullptr);
  check_glerror();
  glCompileShader(shader);
  GLint success = 0;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
  if (success != GL_TRUE) {
    GLint loglen = 0;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &loglen);
    std::vector<GLchar> errors(loglen);
    glGetShaderInfoLog(shader, loglen, &loglen, errors.data());
    throw std::runtime_error(std::string("Shader compilation error: ") +
                             std::string(errors.begin(), errors.end()));
  }
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
