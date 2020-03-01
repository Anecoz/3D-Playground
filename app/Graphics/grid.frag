#version 430

layout (location = 0) out vec4 outColor;

uniform vec3 cameraPos;

in vec3 fragNormal;
flat in vec3 fragColour;
in vec3 fragPosition;

#define M_PI 3.1415926535897932384626433832795

#define BASE_COL_GRASS vec3(91.0/255.0, 255.0/255.0, 26.0/255.0)
#define ALT_COL_GRASS vec3(222.0/255.0, 213.0/255.0, 47.0/255.0)
#define BASE_COL_MOUNTAIN vec3(196.0/255.0, 187.0/255.0, 182.0/255.0)

vec3 calcColor(float height, float angle) {
  if (angle > M_PI/5.0) {
    return BASE_COL_MOUNTAIN;
  }
  //return ALT_COL_GRASS;
  return clamp(mix(BASE_COL_GRASS, ALT_COL_GRASS, angle/(M_PI/6.0)), BASE_COL_GRASS, ALT_COL_GRASS);
}

void main() {
  vec3 normal = normalize(fragNormal);
  vec3 lightColor = vec3(1.0, 1.0, 1.0);
  vec3 lightDir = normalize(vec3(0.5, 0.5, 0.0));

  // Ambient
  float ambientStrength = 0.1;
  vec3 ambient = ambientStrength * lightColor;

  // Diffuse
  float diff = max(dot(normal, lightDir), 0.0);
  vec3 diffuse = diff * lightColor;

  // Specular
  float specularStrength = 0.5;
  vec3 viewDir = normalize(cameraPos - fragPosition);
  vec3 reflectDir = reflect(-lightDir, normal);
  float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
  vec3 specular = specularStrength * spec * lightColor;

  // Calculate angle of normal
  float angle = acos(dot(normal, vec3(0, 1, 0)));
  vec3 col = calcColor(fragPosition.y, angle);

  vec3 result = (ambient + diffuse /*+ specular*/) * col;

  float gamma = 1.8;
  result = pow(result, vec3(1.0/gamma));

  outColor = vec4(result, 1.0);
}