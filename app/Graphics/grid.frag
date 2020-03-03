#version 430

layout (location = 0) out vec4 outColor;

uniform vec3 cameraPos;

flat in vec3 fragNormal;
in vec3 fragPosition;

#define M_PI 3.1415926535897932384626433832795

#define BASE_COL_GRASS vec3(91.0/255.0, 255.0/255.0, 26.0/255.0)
#define ALT_COL_GRASS vec3(222.0/255.0, 213.0/255.0, 47.0/255.0)
#define BASE_COL_MOUNTAIN vec3(196.0/255.0, 187.0/255.0, 182.0/255.0)
#define BASE_COL_WATER vec3(37.0/255.0, 17.0/255.0, 255.0/255.0)

vec3 calcColor(float height, float angle) {
  if (height < -25.0) {
    return BASE_COL_WATER;
  }
  if (angle > M_PI/5.0) {
    return BASE_COL_MOUNTAIN;
  }
  if (height > 40.0) {
    return clamp(mix(ALT_COL_GRASS, ALT_COL_GRASS, height/60.0), BASE_COL_MOUNTAIN, ALT_COL_GRASS);
  }
  //return ALT_COL_GRASS;
  return clamp(mix(BASE_COL_GRASS, ALT_COL_GRASS, angle/(M_PI/6.0)), BASE_COL_GRASS, ALT_COL_GRASS);
}

void main() {
  vec3 normal = normalize(fragNormal);
  vec3 lightDir = normalize(vec3(0.58, 0.58, 0.58));
  //vec3 lightDir = normalize(cameraPos - fragPosition);

  // Calculate angle of normal
  float angle = acos(dot(normal, vec3(0, 1, 0)));
  vec3 fragColor = calcColor(fragPosition.y, angle);

  vec3 ambientColor = vec3(1.0, 1.0, 1.0);
  vec3 diffuseColor = fragColor;
  vec3 specColor = vec3(0.5, 0.5, 0.5);

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

  float gamma = 1.8;
  result = pow(result, vec3(1.0/gamma));

  outColor = vec4(result, 1.0);
}