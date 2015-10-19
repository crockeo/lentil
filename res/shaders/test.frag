#version 330

uniform sampler2D tex;
in vec2 p_tvert;

out vec4 out_color;

void main() {
    out_color = texture(tex, p_tvert);
}
