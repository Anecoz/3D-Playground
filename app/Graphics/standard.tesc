#version 430

layout (vertices = 3) out;

uniform vec3 cameraPos;

in vec3 worldPos_CS_in[];

out vec3 worldPos_ES_in[];

float getTessLevel(float dist0, float dist1) {
  float avg = (dist0 + dist1) / 2.0;

  if (avg <= 5.0) {
    return 20.0;
  }
  else if (avg <= 15.0) {
    return 14.0;
  }
  else if (avg <= 30) {
    return 6.0;
  }
  else {
    return 1.0;
  }
}

void main() {
  worldPos_ES_in[gl_InvocationID] = worldPos_CS_in[gl_InvocationID];

  float distance0 = distance(cameraPos, worldPos_CS_in[0]);
  float distance1 = distance(cameraPos, worldPos_CS_in[1]);
  float distance2 = distance(cameraPos, worldPos_CS_in[2]);

  gl_TessLevelOuter[0] = getTessLevel(distance1, distance2);
  gl_TessLevelOuter[1] = getTessLevel(distance2, distance0);
  gl_TessLevelOuter[2] = getTessLevel(distance0, distance1);
  gl_TessLevelInner[0] = gl_TessLevelOuter[2];
}