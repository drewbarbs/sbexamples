#ifndef MOVINGTRI_H
#define MOVINGTRI_H

#include <GL/gl3w.h>
#include "app.h"
#include "util.h"

class movingtri: public sb::app {
  sb::scope_handle<GLuint> prog;
  sb::scope_handle<GLuint> vao;

  virtual void do_render(double current_time) override;

public:
  movingtri();
  ~movingtri();
};


#endif /* MOVINGTRI_H */
