#version 430

layout (location = 0) out vec4 outColor;

in vec3 fragNormal;
in vec3 fragColor;

void main() {
  vec3 normal = normalize(fragNormal);
  vec3 lightColor = vec3(1.0, 1.0, 1.0);
  vec3 lightDir = -normalize(vec3(1.0, 0.0, 0.0));

  // Ambient
  float ambientStrength = 0.5;
  vec3 ambient = ambientStrength * lightColor;

  // Diffuse
  float diff = max(dot(normal, lightDir), 0.0);
  vec3 diffuse = diff * lightColor;

  vec3 result = (ambient + diffuse) * normalize(fragColor);
  //outColor = vec4(result, 1.0);

  outColor = vec4(result, 1.0);
}