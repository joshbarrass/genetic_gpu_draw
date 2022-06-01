#version 330 core
in float vertAlpha;
in vec3 vertColor;
out vec4 FragColor;
layout(location = 0) out vec4 TexColor;

void main() {
     FragColor = vec4(vertColor.xyz, vertAlpha);
     TexColor = vec4(vertColor.xyz, vertAlpha);
}
