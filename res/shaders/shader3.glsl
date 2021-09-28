#version 330 core


out vec4 FragColor;
in vec4 vertexColor;
in vec2 tc;

uniform vec4 ourColor;
uniform sampler2D tex_colour;

void main() {
    FragColor = texture(tex_colour, tc);
}