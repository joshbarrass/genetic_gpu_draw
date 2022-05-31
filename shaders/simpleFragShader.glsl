#version 330 core
in float vertAlpha;
in vec3 vertColor;
out vec4 FragColor;

void main() {
     FragColor = vec4(vertColor.xyz, vertAlpha);
}
