#include "ShadowMapGenerator.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/transform.hpp>

ShadowMapGenerator::ShadowMapGenerator()
  : _shadowFbo(8192, 8192, true, true)
  , _shadowCamera(glm::vec3(0.0, 0.0, 0.0), ProjectionType::Orthogonal)
  , _shadowTexture(8192, 8192, _shadowFbo.getDepthTextureId(), GL_TEXTURE_2D)
{}

ShadowMapGenerator::~ShadowMapGenerator()
{
}

const Camera& ShadowMapGenerator::prepareShadowPass(const Camera& mainCamera, const glm::vec3& sunDirection)
{
  _shadowFbo.bind();

  auto pos = mainCamera.getPosition();
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