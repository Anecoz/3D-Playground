#version 430

layout (location = 0) in vec3 inPosition;
layout (location = 2) in vec3 inNormal;
layout (location = 3) in vec3 inColor;
layout (location = 4) in mat4 instanceMatrix;

uniform mat4 camMatrix;
uniform mat4 projMatrix;
uniform mat4 modelMatrix = mat4(1.0f);

flat out vec3 geomColor;
out vec3 geomPosition;

void main() {
  geomColor = inColor;
  geomPosition = vec3(instanceMatrix * vec4(inPosition, 1.0));
  
  gl_Position = projMatrix * camMatrix * vec4(geomPosition, 1.0);
}