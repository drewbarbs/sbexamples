#ifndef SBEXAMPLES_UTIL_H
#define SBEXAMPLES_UTIL_H

#include <iostream>
#include <string>
#include <functional>

#include <GL/gl3w.h>

#define check_glerror() sb::_check_glerror(__FILE__, __LINE__)

namespace sb {

  // Use to read glsl shaders
  std::string read_file(const std::string &fname);

  void shader_src_and_compile(const GLuint shader, const std::string &src);

  // Raises exception if getGlError() != GL_NO_ERROR
  void _check_glerror(const char *file, int line);

  // A class for managing OpenGL resources
  template<typename T>
  class scope_handle {
    T handle;
    std::function<void (T)> deleter;

  public:
    scope_handle(T handle,
                 std::function<void (T)> deleter): handle(handle),
                                                       deleter(deleter) {}
    scope_handle(): handle(0), deleter([](T){}) {}
    scope_handle(const scope_handle &) = delete;
    scope_handle(scope_handle &&h) {
      h.swap(*this);
    }

    scope_handle& operator=(const scope_handle &h) = delete;
    scope_handle& operator=(scope_handle &&h) {
      h.swap(*this);
      return *this;
    }

    void swap(scope_handle &other) noexcept {
      using std::swap;
      swap(this->handle, other.handle);
      swap(this->deleter, other.deleter);
    }

    T get() { return handle; }

    explicit operator bool() {
      return !!handle;
    }

    ~scope_handle() {
      deleter(handle);
    }
  };
}

#endif //SBEXAMPLES_UTIL_H
