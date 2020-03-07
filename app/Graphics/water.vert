#version 430

layout (location = 0) in vec3 inPosition;

uniform mat4 camMatrix;
uniform mat4 projMatrix;
uniform mat4 modelMatrix = mat4(1.0f);
uniform float time;

out vec3 geomPosition;

#define M_PI 3.1415926535897932384626433832795

float offset(float x, float z, float val1, float val2, float waveLength) {
  float sinVal = ((mod(x + z*x*val1, waveLength)/waveLength) + time * mod(x * 0.8 + z, 1.5)) * M_PI * 0.7;
  float cosVal = ((mod(val2 * (z*x + x*z), waveLength)/waveLength) + time * 2.0 * mod(x, 2.0)) * M_PI * 0.7;

  float slowSin = sin(M_PI * 0.1 * (x/waveLength + time));
  float slowCos = sin(M_PI * 0.1 * (z/waveLength + time));

  float hiFreqSin = sin(M_PI * 0.3 * ((x * 10.0)/waveLength + time));
  float hiFreqCos = sin(M_PI * 0.3 * ((z * 10.0)/waveLength + time));

  return 0.1 * (sin(sinVal) + cos(cosVal));// + 0.7 * (sin(slowSin) + cos(slowCos)) + 0.5 * (sin(hiFreqSin + cos(hiFreqCos)));
}

void main() {
  geomPosition = vec3(modelMatrix * vec4(inPosition, 1.0));

  float waveLength = 256.0;
  /*geomPosition = geomPosition + vec3(
    offset(geomPosition.x, geomPosition.z, 0.2, 0.1, waveLength),
    offset(geomPosition.x, geomPosition.z, 0.1, 0.3, waveLength),
    offset(geomPosition.x, geomPosition.z, 0.15, 0.2, waveLength));*/

  geomPosition.y += offset(geomPosition.x, geomPosition.z, 0.1, 0.3, waveLength);

  gl_Position = projMatrix * camMatrix * vec4(geomPosition, 1.0);
}