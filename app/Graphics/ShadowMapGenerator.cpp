#include "ShadowMapGenerator.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/transform.hpp>

#include <iostream>
#include <vector>

ShadowMapGenerator::ShadowMapGenerator()
  : _shadowFbo(4096, 4096, true, true)
  , _shadowCamera(glm::vec3(0.0, 0.0, 0.0), ProjectionType::Orthogonal)
  , _shadowTexture(4096, 4096, _shadowFbo.getDepthTextureId(), GL_TEXTURE_2D)
{}

ShadowMapGenerator::~ShadowMapGenerator()
{
}

const Camera& ShadowMapGenerator::prepareShadowPass(const Camera& mainCamera, const glm::vec3& sunDirection)
{
  _shadowFbo.bind();

  // Calculates a "light" projection and view matrix that is perfectly aligned to the main cameras frustum.
  // Calculate frustum 8 points in world space
  auto ndcToWorldCam = glm::inverse(mainCamera.getCombined());

  std::vector<glm::vec3> ndcs;
  ndcs.emplace_back(-1.0, -1.0, -1.0);
  ndcs.emplace_back(-1.0, -1.0,  1.0);
  ndcs.emplace_back(-1.0,  1.0,  1.0);
  ndcs.emplace_back( 1.0,  1.0,  1.0);
  ndcs.emplace_back( 1.0, -1.0,  1.0);
  ndcs.emplace_back( 1.0, -1.0, -1.0);
  ndcs.emplace_back(-1.0,  1.0, -1.0);
  ndcs.emplace_back( 1.0,  1.0, -1.0);

  std::vector<glm::vec4> cornersWorld;
  glm::vec4 midPoint(0.0, 0.0, 0.0, 0.0);
  for (auto& ndc: ndcs) {
    glm::vec4 cornerWorld = ndcToWorldCam * glm::vec4(ndc, 1.0);
    cornersWorld.emplace_back(cornerWorld.x / cornerWorld.w, cornerWorld.y / cornerWorld.w, cornerWorld.z / cornerWorld.w, 1.0);
    midPoint += cornersWorld.back();
  }
  midPoint /= 8.0;

  glm::mat4 lvMatrix = glm::lookAt(glm::vec3(midPoint) - glm::normalize(sunDirection), glm::vec3(midPoint), glm::vec3(0.0f, 1.0f, 0.0f));

  glm::vec4 transf = lvMatrix * cornersWorld[0];
  float minZ = transf.z;
  float maxZ = transf.z;
  float minX = transf.x;
  float maxX = transf.x;
  float minY = transf.y;
  float maxY = transf.y;

  for (unsigned int i = 1; i < 8; i++) {
    transf = lvMatrix * cornersWorld[i];

    if (transf.z > maxZ) maxZ = transf.z;
    if (transf.z < minZ) minZ = transf.z;
    if (transf.x > maxX) maxX = transf.x;
    if (transf.x < minX) minX = transf.x;
    if (transf.y > maxY) maxY = transf.y;
    if (transf.y < minY) minY = transf.y;
  }

  glm::mat4 lpMatrix = glm::ortho(minX, maxX, minY, maxY, minZ, maxZ);

  _shadowCamera.setViewMatrix(lvMatrix);
  _shadowCamera.setProjection(lpMatrix);

  return _shadowCamera;
}

void ShadowMapGenerator::endShadowPass()
{
  _shadowFbo.unbind();
}

const Texture& ShadowMapGenerator::getShadowTexture()
{
  return _shadowTexture;
}