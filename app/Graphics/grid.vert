#version 430

layout (location = 0) in vec3 inPosition;

uniform mat4 camMatrix;
uniform mat4 projMatrix;
uniform mat4 modelMatrix = mat4(1.0f);
uniform mat4 shadowMatrix;

out vec3 geomPosition;
out vec4 geomShadowSpacePosition;

void main() {
  geomPosition = vec3(modelMatrix * vec4(inPosition, 1.0));
  geomShadowSpacePosition = shadowMatrix * vec4(geomPosition, 1.0);

  gl_Position = projMatrix * camMatrix * modelMatrix * vec4(inPosition, 1.0);
}