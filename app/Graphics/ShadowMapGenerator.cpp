#include "ShadowMapGenerator.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/transform.hpp>

#include <iostream>
#include <vector>

ShadowMapGenerator::ShadowMapGenerator()
  : _shadowFbo(8192, 8192, true, 1)
  , _shadowCamera(glm::vec3(0.0, 0.0, 0.0), ProjectionType::Orthogonal)
  , _shadowTexture(8192, 8192, _shadowFbo.getDepthTextureId(0), GL_TEXTURE_2D)
{}

ShadowMapGenerator::~ShadowMapGenerator()
{
}

const Camera& ShadowMapGenerator::prepareShadowPass(const Camera& mainCamera, const glm::vec3& sunDirection, const float& testFarMax)
{
  _shadowFbo.bind(0);

  // Calculates a "light" projection and view matrix that is aligned to the main cameras frustum.
  // Calculate frustum 8 points in world space
  auto ndcToWorldCam = glm::inverse(mainCamera.getCombined());
  auto ndcToMainCamView = glm::inverse(mainCamera.getProjection());

  const double ndcMin = -1.0;
  const double ndcMax = 1.0;
  glm::vec3 nearLeftBottom(ndcMin, ndcMin, ndcMin);
  glm::vec3 farLeftBottom(ndcMin, ndcMin, ndcMax);
  glm::vec3 farRightBottom(ndcMin, ndcMax, ndcMax);
  glm::vec3 farRightTop(ndcMax, ndcMax, ndcMax);
  glm::vec3 farLeftTop(ndcMax, ndcMin, ndcMax);
  glm::vec3 nearLeftTop(ndcMax, ndcMin, ndcMin);
  glm::vec3 nearRightBottom(ndcMin, ndcMax, ndcMin);
  glm::vec3 nearRightTop(ndcMax, ndcMax, ndcMin);

  std::vector<glm::vec4> cornersWorld;
  glm::vec4 midPoint(0.0, 0.0, 0.0, 0.0);

  glm::vec4 wNearLeftBottom = ndcToWorldCam * glm::vec4(nearLeftBottom, 1.0);
  wNearLeftBottom /= wNearLeftBottom.w;
  glm::vec4 wNearLeftTop = ndcToWorldCam * glm::vec4(nearLeftTop, 1.0);
  wNearLeftTop /= wNearLeftTop.w;
  glm::vec4 wNearRightBottom = ndcToWorldCam * glm::vec4(nearRightBottom, 1.0);
  wNearRightBottom /= wNearRightBottom.w;
  glm::vec4 wNearRightTop = ndcToWorldCam * glm::vec4(nearRightTop, 1.0);
  wNearRightTop /= wNearRightTop.w;

  glm::vec4 tmp;
  const float percentageFar = testFarMax;

  glm::vec4 wFarLeftBottom = ndcToWorldCam * glm::vec4(farLeftBottom, 1.0);
  wFarLeftBottom /= wFarLeftBottom.w;
  tmp = wFarLeftBottom - wNearLeftBottom;
  wFarLeftBottom = wNearLeftBottom + glm::normalize(tmp) * percentageFar * glm::length(tmp);

  glm::vec4 wFarRightBottom = ndcToWorldCam * glm::vec4(farRightBottom, 1.0);
  wFarRightBottom /= wFarRightBottom.w;
  tmp = wFarRightBottom - wNearRightBottom;
  wFarRightBottom = wNearRightBottom + glm::normalize(tmp) * percentageFar * glm::length(tmp);

  glm::vec4 wFarRightTop = ndcToWorldCam * glm::vec4(farRightTop, 1.0);
  wFarRightTop /= wFarRightTop.w;
  tmp = wFarRightTop - wNearRightTop;
  wFarRightTop = wNearRightTop + glm::normalize(tmp) * percentageFar * glm::length(tmp);

  glm::vec4 wFarLeftTop = ndcToWorldCam * glm::vec4(farLeftTop, 1.0);
  wFarLeftTop /= wFarLeftTop.w;
  tmp = wFarLeftTop - wNearLeftTop;
  wFarLeftTop = wNearLeftTop + glm::normalize(tmp) * percentageFar * glm::length(tmp);

  cornersWorld.emplace_back(wNearLeftBottom);
  cornersWorld.emplace_back(wNearLeftTop);
  cornersWorld.emplace_back(wNearRightBottom);
  cornersWorld.emplace_back(wNearRightTop);
  cornersWorld.emplace_back(wFarLeftBottom);
  cornersWorld.emplace_back(wFarRightBottom);
  cornersWorld.emplace_back(wFarRightTop);
  cornersWorld.emplace_back(wFarLeftTop);

  midPoint = wNearLeftBottom + wNearLeftTop + wNearRightBottom + wNearRightTop + wFarLeftBottom + wFarRightBottom + wFarRightTop + wFarLeftTop;
  midPoint /= 8.0;

  /*for (auto& ndc: ndcs) {
    glm::vec4 cornerWorld = ndcToWorldCam * glm::vec4(ndc.x, ndc.y, ndc.z, 1.0);
    cornersWorld.emplace_back(cornerWorld.x / cornerWorld.w, cornerWorld.y / cornerWorld.w, cornerWorld.z / cornerWorld.w, 1.0);
    midPoint += cornersWorld.back();
  }
  midPoint /= 8.0;*/

  glm::mat4 lvMatrix = glm::lookAt(glm::vec3(midPoint) - glm::normalize(sunDirection), glm::vec3(midPoint), glm::vec3(0.0, 1.0, 0.0));

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