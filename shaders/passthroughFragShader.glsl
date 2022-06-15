#version 330 core
layout(location = 0) out vec4 TexColor;

uniform sampler2D target_image;
uniform vec2 image_size;

void main() {
     vec2 uv = (gl_FragCoord.xy) / image_size;
     vec3 imageColor = texture(target_image, uv).xyz;
     TexColor = vec4(imageColor, 1.0);
}
