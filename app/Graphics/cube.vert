#version 430

layout (location = 0) in vec3 inPosition;
layout (location = 2) in vec3 inNormal;
layout (location = 3) in vec3 inColor;

uniform mat4 camMatrix;
uniform mat4 projMatrix;
uniform mat4 modelMatrix = mat4(1.0f);

out vec3 fragNormal;
out vec3 fragColor;

void main() {
  fragNormal = inNormal;
  fragColor = inColor;
  gl_Position = projMatrix * camMatrix * modelMatrix * vec4(inPosition, 1.0);
}