#ifndef TESSTRI_H
#define TESSTRI_H

#include <GL/gl3w.h>
#include "app.h"
#include "util.h"

class tesstri: public sb::app {
  sb::scope_handle<GLuint> prog;
  sb::scope_handle<GLuint> vao;

  virtual void do_render(double current_time) override;

public:
  tesstri();
  ~tesstri();
};


#endif /* TESSTRI_H */
