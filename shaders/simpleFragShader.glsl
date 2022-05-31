#version 330 core
in float vertAlpha;
out vec4 FragColor;

uniform vec3 triColor;

void main() {
     FragColor = vec4(triColor, vertAlpha);
}
