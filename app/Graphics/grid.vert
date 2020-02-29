#version 430

layout (location = 0) in vec3 inPosition;
layout (location = 2) in vec3 inNormal;

uniform mat4 camMatrix;
uniform mat4 projMatrix;
uniform mat4 modelMatrix = mat4(1.0f);

out vec3 fragNormal;
flat out vec3 fragColour;
out vec3 fragPosition;

vec3 calcColor(float height, float angle) {
  if (height > 40) {
    return vec3(196.0/255.0, 187.0/255.0, 182.0/255.0);
  }
  return vec3(91.0/255.0, 255.0/255.0, 26.0/255.0);
}

void main() {
  fragNormal = inNormal;
  fragPosition = vec3(modelMatrix * vec4(inPosition, 1.0));

  // Calculate angle of normal
  float angle = acos(dot(normalize(fragNormal), vec3(0, 1, 0)));
  fragColour = calcColor(fragPosition.y, angle);

  gl_Position = projMatrix * camMatrix * modelMatrix * vec4(inPosition, 1.0);
}