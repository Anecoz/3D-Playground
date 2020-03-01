#version 430

layout (location = 0) in vec3 inPosition;
layout (location = 2) in vec3 inNormal;
layout (location = 3) in vec3 inColor;
layout (location = 4) in vec3 inOffsets;

uniform mat4 camMatrix;
uniform mat4 projMatrix;
uniform mat4 modelMatrix = mat4(1.0f);

flat out vec3 fragNormal;
out vec3 fragColor;
out vec3 fragPosition;

void main() {
  fragNormal = inNormal;
  fragColor = inColor;
  fragPosition = vec3(modelMatrix * vec4(inOffsets + inPosition, 1.0));
  gl_Position = projMatrix * camMatrix * modelMatrix * vec4(inOffsets + inPosition, 1.0);
}