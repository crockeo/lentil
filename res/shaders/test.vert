#version 330

uniform mat3 camera_matrix;
uniform vec2 window_size;
uniform float scale;

in vec4 pvert;
in vec3 tvert;

out vec2 p_tvert;

void main() {
    vec4 tpos = vec4(
        pvert.x,
        pvert.y * (window_size.x / window_size.y),
        pvert.z,
        1 / scale
    );

    tpos = vec4(tpos.xyz * camera_matrix, tpos.w);
    gl_Position = tpos;

    p_tvert = tvert.xy;
}
