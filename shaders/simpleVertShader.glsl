#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in float alpha;
layout (location = 2) in vec2 COM;

uniform sampler2D target_image;

out float vertAlpha;
out vec3 vertColor;

void main() {
  gl_Position = vec4(aPos.xyz, 1.0);
  vertAlpha = alpha;
  vec2 adjustedCOM = (COM.xy + 1.0)/2.0;
  vertColor = texture(target_image, adjustedCOM).xyz;
}
