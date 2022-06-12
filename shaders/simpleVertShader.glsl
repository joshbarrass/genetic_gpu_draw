#version 330 core
layout (location = 0) in vec2 aPos;

uniform sampler2D target_image;

void main() {
  gl_Position = vec4(aPos.xy, 0.0, 1.0);
}
