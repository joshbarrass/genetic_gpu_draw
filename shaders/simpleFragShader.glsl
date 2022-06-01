#version 330 core
in float vertAlpha;
in vec3 vertColor;
layout(location = 0) out vec4 TexColor;

void main() {
     TexColor = vec4(vertColor.xyz, vertAlpha);
}
