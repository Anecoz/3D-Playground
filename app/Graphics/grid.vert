#version 430

layout (location = 0) in vec3 inPosition;
layout (location = 2) in vec3 inNormal;

uniform mat4 camMatrix;
uniform mat4 projMatrix;
uniform mat4 modelMatrix = mat4(1.0f);

out vec3 fragNormal;
out vec3 fragPosition;

void main() {
  fragNormal = inNormal;
  fragPosition = vec3(modelMatrix * vec4(inPosition, 1.0));
  gl_Position = projMatrix * camMatrix * modelMatrix * vec4(inPosition, 1.0);
}