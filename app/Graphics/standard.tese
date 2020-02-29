#version 430

layout(triangles, equal_spacing, ccw) in;

uniform mat4 camMatrix;
uniform mat4 projMatrix;

in vec3 worldPos_ES_in[];

out vec3 worldPos_FS_in;

vec3 interpolate3D(vec3 v0, vec3 v1, vec3 v2) {
  return vec3(gl_TessCoord.x) * v0 + vec3(gl_TessCoord.y) * v1 + vec3(gl_TessCoord.z) * v2;
}

void main() {
  worldPos_FS_in = interpolate3D(worldPos_ES_in[0], worldPos_ES_in[1], worldPos_ES_in[2]);
  gl_Position = projMatrix * camMatrix * vec4(worldPos_FS_in, 1.0);
}