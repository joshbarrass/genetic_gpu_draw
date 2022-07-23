#version 330 core
layout(location = 0) out vec4 TexColor;

uniform sampler2D target_image;
uniform vec2 image_size;
uniform vec2 sample_coord;

void main() {
     vec3 imageColor = texture(target_image, sample_coord).xyz;
     TexColor = vec4(imageColor, 1.0);
}
