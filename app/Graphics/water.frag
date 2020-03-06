#version 430

layout (location = 0) out vec4 outColor;

uniform vec3 cameraPos;

flat in vec3 fragNormal;
in vec3 fragPosition;

#define M_PI 3.1415926535897932384626433832795

const float fresnelReflective = 1.2;

float calculateFresnel() {
  vec3 viewVector = normalize(cameraPos - fragPosition);
  vec3 normal = normalize(fragNormal);
  float refractiveFactor = dot(viewVector, normal);
  refractiveFactor = pow(refractiveFactor, fresnelReflective);
  return clamp(refractiveFactor, 0.0, 1.0);
}

void main() {
  vec3 result = vec3(57.0/255.0, 202.0/255.0, 255.0/255.0);

  float gamma = 1.8;
  result = pow(result, vec3(1.0/gamma));

  outColor = vec4(result, 1.0 - calculateFresnel());
}