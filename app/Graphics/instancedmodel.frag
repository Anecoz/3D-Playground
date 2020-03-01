#version 430

layout (location = 0) out vec4 outColor;

flat in vec3 fragNormal;
flat in vec3 fragColor;
in vec3 fragPosition;

uniform vec3 cameraPos;

void main() {
  vec3 ambientColor = vec3(1.0, 1.0, 1.0);
  vec3 diffuseColor = fragColor;
  vec3 specColor = vec3(0.5, 0.5, 0.5);

  vec3 normal = normalize(fragNormal);
  //vec3 lightDir = normalize(cameraPos - fragPosition);
  vec3 lightDir = normalize(vec3(0.58, 0.58, 0.58));

  // Ambient
  float ambientStrength = 0.1;
  vec3 ambient = ambientStrength * ambientColor;

  // Diffuse
  float diff = max(dot(normal, lightDir), 0.0);
  vec3 diffuse = diff * diffuseColor;

  // Specular
  vec3 viewDir = normalize(cameraPos - fragPosition);
  vec3 reflectDir = reflect(-lightDir, normal);  
  float spec = pow(max(dot(viewDir, reflectDir), 0.0), 324);
  vec3 specular = 0.1 * spec * specColor;

  vec3 result = (ambient + diffuse + specular);
  //outColor = vec4(normal, 1.0);

  float gamma = 1.8;
  result = pow(result, vec3(1.0/gamma));

  outColor = vec4(result, 1.0);
}