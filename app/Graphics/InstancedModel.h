#pragma once

#include "Lowlevel/Shader.h"

#include <glm/glm.hpp>

#include <memory>
#include <string>

class Camera;
class IndexedVertexArray;

class InstancedModel
{
public:
  InstancedModel(const std::string& objPath);
  ~InstancedModel();

  void draw(const Camera& camera);

private:
  Shader _shader;
  std::unique_ptr<IndexedVertexArray> _mesh;
  glm::mat4 _modelMatrix;
};