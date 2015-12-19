#version 330

#define M_PI 3.14159

uniform vec3 camera_position;
uniform vec2 camera_rotation;
uniform vec2 window_size;
uniform float scale;

in vec4 pvert;
in vec3 tvert;

out vec2 p_tvert;

// Constructing the original position.
vec4 start_pos() {
   return vec4(vec3(pvert.x, pvert.y * (window_size.x / window_size.y), pvert.z) - camera_position, 1 / scale);
}

// Constructing the rotational vertices.
mat4 x_matrix(vec3 r) {
    return mat4(
        1,         0,          0, 0,
        0, cos(-r.x), -sin(-r.x), 0,
        0, cos(-r.x),  cos(-r.x), 0,
        0,         0,          0, 1
    );
}

mat4 y_matrix(vec3 r) {
    return mat4(
         cos(-r.y), 0, sin(-r.y), 0,
                 0, 1,         0, 0,
        -sin(-r.y), 0, cos(-r.y), 0,
                 0, 0,         0, 1
    );
}

void main() {
    vec3 cr = vec3(camera_rotation.x, camera_rotation.y, 0);
    vec4 tp = start_pos() * x_matrix(cr) * y_matrix(cr);

    tp.w += tp.z / 2;
    tp.z /= 100;

    gl_Position = tp;

    p_tvert = tvert.xy;
}
