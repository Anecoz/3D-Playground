#version 430

layout (location = 0) out vec4 outColor;

uniform vec3 cameraPos;

flat in vec3 fragNormal;
in vec3 fragPosition;

#define M_PI 3.1415926535897932384626433832795

void main() {
  vec3 result = vec3(57.0/255.0, 202.0/255.0, 255.0/255.0);

  float gamma = 1.8;
  result = pow(result, vec3(1.0/gamma));

  outColor = vec4(result, 1.0);
}