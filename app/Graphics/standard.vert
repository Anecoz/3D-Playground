#version 430

layout (location = 0) in vec3 inPosition;

//uniform mat4 camMatrix;
//uniform mat4 projMatrix;
uniform mat4 modelMatrix = mat4(1.0f);

out vec3 worldPos_CS_in;

void main() {
  worldPos_CS_in = (modelMatrix * vec4(inPosition, 1.0)).xyz;
  //gl_Position = projMatrix * camMatrix * modelMatrix * vec4(inPosition, 1.0);
}