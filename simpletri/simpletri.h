#ifndef SBEXAMPLES_SIMPLETRI_H
#define SBEXAMPLES_SIMPLETRI_H

#include <GL/gl3w.h>
#include "app.h"

class simpletri : public sb::app {
    GLuint prog = 0;
    GLuint vao = 0;
    virtual void do_render(double time) override;

public:
    simpletri();
    ~simpletri();
};

#endif //SBEXAMPLES_SIMPLETRI_H
