#version 450 core

// "in frag shaders, the value of output variables goes directly onto the screen"
out vec4 color;

void main(void) {
    color = vec4(0.0, 0.8, 1.0, 1.0);
}