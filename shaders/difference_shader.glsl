#version 330 core

layout(location = 0) out vec4 TexColor;

uniform sampler2D target;
uniform sampler2D canvas;
uniform vec2 screen_size;

void main() {
     vec2 uv = (gl_FragCoord.xy) / screen_size;
     vec3 targetColor = texture(target, uv).xyz;
     vec3 canvasColor = texture(canvas, uv).xyz;
     vec3 diff = canvasColor - targetColor;
     TexColor = vec4(dot(diff, diff), 0.0, 0.0, 1.0);
}