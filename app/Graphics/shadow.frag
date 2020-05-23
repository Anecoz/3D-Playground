#version 430

layout (location = 0) out vec4 outColor;

uniform vec3 camPosition;

in vec3 fragPosition;

void main() {
  // Doesn't matter
  float depth = 0.0;//length(fragPosition - camPosition);
  outColor = vec4(vec3(depth), 1.0);
}