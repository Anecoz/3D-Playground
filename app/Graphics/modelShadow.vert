#version 430

layout (location = 0) in vec3 inPosition;
layout (location = 4) in mat4 instanceMatrix;

uniform mat4 camMatrix;
uniform mat4 projMatrix;
uniform mat4 modelMatrix = mat4(1.0f);

out vec3 fragPosition;

void main() {
  fragPosition = vec3(instanceMatrix * vec4(inPosition, 1.0));
  
  gl_Position = projMatrix * camMatrix * vec4(fragPosition, 1.0);
}