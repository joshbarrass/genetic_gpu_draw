#version 330 core
in vec3 vertColor;
layout(location = 0) out vec4 TexColor;

void main() {
     TexColor = vec4(vertColor.xyz, 1.0);
}
