#version 430

layout (location = 0) out vec4 outColor;

uniform vec3 cameraPos;
uniform sampler2D shadowMap;

flat in vec3 fragNormal;
in vec3 fragPosition;
in vec4 fragShadowSpacePosition;

#define M_PI 3.1415926535897932384626433832795

#define BASE_COL_GRASS vec3(91.0/255.0, 255.0/255.0, 26.0/255.0)
#define ALT_COL_GRASS vec3(222.0/255.0, 213.0/255.0, 47.0/255.0)
#define BASE_COL_MOUNTAIN vec3(196.0/255.0, 187.0/255.0, 182.0/255.0)
#define BASE_COL_MUD vec3(133.0/255.0, 99.0/255.0, 11.0/255.0)

float shadowCalc() {
  vec3 projCoords = fragShadowSpacePosition.xyz / fragShadowSpacePosition.w;
  // transform to [0,1] range
  projCoords = projCoords * 0.5 + 0.5;
  if (projCoords.x > 1.0 || projCoords.x < 0.0 ||
      projCoords.z > 1.0 || projCoords.z < 0.0) {
    return 0.0;
  }

  // get depth of current fragment from light's perspective
  float currentDepth = projCoords.z;

  float bias = 0.005;
  float texelSize = 1.0 / textureSize(shadowMap, 0).x;
  float total = 0.0;

  int sampleStep = 3;
  float sampleCount = (sampleStep * 2.0 + 1.0) * (sampleStep * 2.0 + 1.0);

  for (int x = -sampleStep; x <= sampleStep; ++x) {
    for (int y = -sampleStep; y <= sampleStep; ++y) {
      float closestDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r;
      if (currentDepth - bias > closestDepth) {
        total += 1.0;
      }
    }
  }

  total /= sampleCount;
  return total;
}

vec3 calcColor(float height, float angle) {
  vec3 baseCol = clamp(mix(BASE_COL_GRASS, ALT_COL_GRASS, angle/(M_PI/6.0)), BASE_COL_GRASS, ALT_COL_GRASS);
  float mixFactor = smoothstep(-18.0, -15.0, height);
  baseCol = clamp(mix(BASE_COL_MUD, baseCol, mixFactor), BASE_COL_MUD, baseCol);

  if (height < -18.0) {
    return BASE_COL_MUD;
  }
  if (angle > M_PI/5.0) {
    return BASE_COL_MOUNTAIN;
  }
  if (height > 40.0) {
    return clamp(mix(ALT_COL_GRASS, ALT_COL_GRASS, height/60.0), BASE_COL_MOUNTAIN, ALT_COL_GRASS);
  }
  //return ALT_COL_GRASS;
  return baseCol;
}

float toLinearDepth(float zDepth)
{
  float near = 0.1;
  float far = 1000.0;
  return 2.0 * near * far / (far + near - (2.0 * zDepth - 1.0) * (far - near));
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

  float shadow = shadowCalc();
  vec3 result = ambient + (1.0 - shadow) * (diffuse + specular);

  float gamma = 1.8;
  result = pow(result, vec3(1.0/gamma));
  outColor = vec4(result, 1.0);
}