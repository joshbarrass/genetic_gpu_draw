#version 330 core

layout(location = 0) out vec4 TexColor;

uniform Texture2D target;
uniform Texture2D canvas;

void main() {
     vec3 targetColor = texture(target, gl_FragCoord.xy).xyz;
     vec3 canvasColor = texture(canvas, gl_FragCoord.xy).xyz;
     vec3 diff = targetColor - canvasColor;
     TexColor = vec4(diff.dot(diff), 0.0, 0.0, 1.0);
}