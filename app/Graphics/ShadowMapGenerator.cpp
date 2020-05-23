#include "ShadowMapGenerator.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/transform.hpp>

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

  // Try something else

  // Calculate frustum 8 points in world space
  /*auto ndcToWorldCam = glm::inverse(mainCamera.getCombined());

  std::vector<glm::vec3> ndcs;
  ndcs.emplace_back(-1.0, -1.0, -1.0);
  ndcs.emplace_back(-1.0, -1.0,  1.0);
  ndcs.emplace_back(-1.0,  1.0,  1.0);
  ndcs.emplace_back( 1.0,  1.0,  1.0);
  ndcs.emplace_back( 1.0, -1.0,  1.0);
  ndcs.emplace_back( 1.0, -1.0, -1.0);
  ndcs.emplace_back(-1.0,  1.0, -1.0);
  ndcs.emplace_back( 1.0,  1.0, -1.0);

  std::vector<glm::vec3> cornersWorld;
  for (auto& ndc: ndcs) {
    glm::vec4 cornerWorld = ndcToWorldCam * glm::vec4(ndc, 1.0);
    cornersWorld.emplace_back(cornerWorld.x / cornerWorld.w, cornerWorld.y / cornerWorld.w, cornerWorld.z / cornerWorld.w);
  }

  double minX = 1e20;
  double minY = 1e20;
  double minZ = 1e20;
  double maxX = -1e20;
  double maxY = -1e20;
  double maxZ = -1e20;

  for (auto& cornerWorld: cornersWorld) {
    if (cornerWorld.x < minX) {
      minX = cornerWorld.x;
    }
    if (cornerWorld.y < minY) {
      minY = cornerWorld.y;
    }
    if (cornerWorld.z < minZ) {
      minZ = cornerWorld.z;
    }
    if (cornerWorld.x > maxX) {
      maxX = cornerWorld.x;
    }
    if (cornerWorld.y > maxY) {
      maxY = cornerWorld.y;
    }
    if (cornerWorld.z > maxZ) {
      maxZ = cornerWorld.z;
    }
  }

  glm::vec3 center((minX + maxX) / 2.0, (minY + maxY) / 2.0, (minZ + maxZ) / 2.0);
  std::cout << "min, max: " << minX << ", " << maxX << ", " << minY << ", " <<  maxY << ", " <<  minZ << ", " <<  maxZ << std::endl;
  _shadowCamera.setProjection(glm::ortho(minX - center.x, maxX - center.x, minY - center.y, maxY - center.y, minZ - center.z, maxZ - center.z));
  //_shadowCamera.setViewMatrix(glm::translate(-center));
  _shadowCamera.setViewMatrix(glm::lookAt(center, center + glm::normalize(sunDirection), glm::vec3(0.0, 1.0, 0.0)));*/

  // Old way
  auto pos = mainCamera.getPosition();
  auto forward = mainCamera.getForward();
  glm::vec3 eye = pos - sunDirection * 50.0f;
  glm::mat4 viewMat = glm::lookAt(eye, pos, glm::vec3(0.0, 1.0, 0.0));
  _shadowCamera.setViewMatrix(viewMat);

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