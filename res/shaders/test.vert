#version 330

#define M_PI 3.14159

uniform vec3 camera_position;
uniform vec2 camera_rotation;
uniform vec2 window_size;
uniform float scale;

in vec4 pvert;
in vec3 tvert;

out vec2 p_tvert;

// Generating the matrix to rotate around the y axis.
mat4 y_matrix(float rot) {
    float rrot = rot / (180 / M_PI);

    return mat4(
         cos(rrot), 0, sin(rrot), 0,
                 0, 1, 0,         0,
        -sin(rrot), 0, cos(rrot), 0,
                 0, 0, 0,         1
    );
}

// Generating the matrix to rotate around the x axis.
mat4 x_matrix(float rot) {
    float rrot = rot / (180 / M_PI);

    return mat4(
        1, 0,         0,          0,
        0, cos(rrot), -sin(rrot), 0,
        0, sin(rrot), cos(rrot),  0,
        0, 0,         0,          1
    );
}

void main() {
    vec4 tpos = vec4(
        pvert.x,
        pvert.y * (window_size.x / window_size.y),
        pvert.z,
        1 / scale
    );

    // Translating depending on the camera.
    tpos = vec4(tpos.xyz - camera_position, tpos.w);

    // Rotating the matrix.
    tpos *= y_matrix(camera_rotation.x);
    tpos *= x_matrix(camera_rotation.y);

    // Setting perspective scaling.
    tpos.w = 2 + tpos.z;

    gl_Position = tpos;

    p_tvert = tvert.xy;
}
