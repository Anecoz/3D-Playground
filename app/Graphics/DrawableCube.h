#pragma once

#include "Lowlevel/IndexedVertexArray.h"
#include "Lowlevel/Shader.h"

#include <glm/glm.hpp>

#include <memory>

class Camera;

class DrawableCube
{
public:
  DrawableCube();

  void draw(const Camera& camera);
private:
  std::unique_ptr<IndexedVertexArray> _cubeMesh;
  Shader _shader;

  glm::mat4 _modelMatrix;
};