#version 430

layout (location = 0) out vec4 outColor;

uniform vec3 cameraPos;

in vec3 fragNormal;
in vec3 fragPosition;

void main() {
  vec3 lightColor = vec3(0.7, 0.7, 0.7);
  vec3 lightDir = normalize(vec3(0.5, 0.5, 0.0));

  // Ambient
  float ambientStrength = 0.1;
  vec3 ambient = ambientStrength * lightColor;

  // Diffuse
  float diff = max(dot(fragNormal, lightDir), 0.0);
  vec3 diffuse = diff * lightColor;

  // Specular
  float specularStrength = 0.5;
  vec3 viewDir = normalize(cameraPos - fragPosition);
  vec3 reflectDir = reflect(-lightDir, fragNormal);
  float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
  vec3 specular = specularStrength * spec * lightColor;

  vec3 result = ambient + diffuse + specular;
  outColor = vec4(result, 1.0);
}