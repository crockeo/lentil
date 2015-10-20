#version 330

uniform vec2 window_size;
uniform float scale;

in vec4 pvert;
in vec3 tvert;

out vec2 p_tvert;

void main() {
    gl_Position = vec4(
        pvert.x,
        pvert.y * (window_size.x / window_size.y),
        pvert.z,
        1 / scale
    );

    p_tvert = tvert.xy;
}
