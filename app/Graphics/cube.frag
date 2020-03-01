#version 430

layout (location = 0) out vec4 outColor;

flat in vec3 fragNormal;
in vec3 fragColor;
in vec3 fragPosition;

uniform vec3 cameraPos;

void main() {
  vec3 normal = normalize(fragNormal);
  vec3 lightColor = vec3(1.0, 1.0, 1.0);
  vec3 lightDir = normalize(vec3(0.58, 0.58, 0.58));

  // Ambient
  float ambientStrength = 0.2;
  vec3 ambient = ambientStrength * lightColor;

  // Diffuse
  float diff = max(dot(normal, lightDir), 0.0);
  vec3 diffuse = diff * fragColor;

  // Specular
  vec3 viewDir = normalize(cameraPos - fragPosition);
  vec3 reflectDir = reflect(-lightDir, normal);  
  float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
  vec3 specular = 0.5 * spec * lightColor;

  vec3 result = (ambient + diffuse + specular) * fragColor;
  //outColor = vec4(result, 1.0);

  outColor = vec4(result, 1.0);
}