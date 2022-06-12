#version 330 core

layout(location = 0) out vec4 TexColor;

uniform sampler2D difference;
uniform vec2 screen_size;

// sum columns into a single row 
void main() {
     vec2 output_image_size = vec2(screen_size.x, 1);
     vec2 uv = (gl_FragCoord.xy) / output_image_size;
     float total = 0.0;
     for (int y = 0; y < screen_size.y; ++y) {
         total += texture(difference, vec2(uv.x, y / screen_size.y)).r;
     }
     TexColor = vec4(total, 0.0, 0.0, 1.0);
}