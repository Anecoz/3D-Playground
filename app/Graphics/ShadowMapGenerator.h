#pragma once

#include "Lowlevel/Fbo.h"
#include "Lowlevel/Texture.h"
#include "Camera.h"

#include <GL/glew.h>

class ShadowMapGenerator
{
public:
  ShadowMapGenerator();
  ~ShadowMapGenerator();

  const Camera& prepareShadowPass(const Camera& mainCamera, const glm::vec3& sunDirection);
  void endShadowPass();

  const Texture& getShadowTexture();

private:
  Fbo _shadowFbo;
  Camera _shadowCamera;
  Texture _shadowTexture;
};