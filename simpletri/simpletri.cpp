#include "simpletri.h"
#include "util.h"

void simpletri::do_render(double time) {
    static const GLfloat green[] = {0.0f, 0.25f, 0.0f, 1.0f};
    glClearBufferfv(GL_COLOR, 0, green);

    glUseProgram(prog);
    glBindVertexArray(vao);
    // Note that we're drawing without a bound array buffer, which is allowed in core profile
    // http://renderingpipeline.com/2012/03/attribute-less-rendering/
    // https://www.opengl.org/wiki_132/index.php/Vertex_Rendering
    glDrawArrays(GL_TRIANGLES, 0, 3);
}

simpletri::simpletri() {
    const char *c_str = nullptr;

    auto vert_shader_src = sb::read_file("vert.glsl");
    c_str = vert_shader_src.c_str();
    GLuint vert_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vert_shader, 1, &c_str, nullptr);
    glCompileShader(vert_shader);

    auto frag_shader_src = sb::read_file("frag.glsl");
    c_str = frag_shader_src.c_str();
    GLuint frag_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(frag_shader, 1, &c_str, nullptr);
    glCompileShader(frag_shader);

    prog = glCreateProgram();
    glAttachShader(prog, vert_shader);
    glAttachShader(prog, frag_shader);
    glLinkProgram(prog);

    glDeleteShader(vert_shader);
    glDeleteShader(frag_shader);

    glGenVertexArrays(1, &vao);
}

simpletri::~simpletri() {
    glDeleteProgram(prog);
    glDeleteVertexArrays(1, &vao);
}
