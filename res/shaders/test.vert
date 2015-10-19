#version 330

in vec4 pvert;
in vec3 tvert;

out vec2 p_tvert;

void main() {
    gl_Position = pvert;
    p_tvert = tvert.xy;
}
