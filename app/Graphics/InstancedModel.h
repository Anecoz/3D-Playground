#pragma once

#include "Lowlevel/Shader.h"

#include <glm/glm.hpp>

#include <cstddef>
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

  void setInstanceMatrices(std::vector<glm::mat4>&& offsets);
  void setNumInstances(std::size_t numInstances);
  void setRotation(float xDeg, float yDeg, float zDeg);
  void setCenter(const glm::vec3& center);

  glm::vec3 getCenter();

private:
  glm::vec3 _center;
  std::size_t _numInstances;
  Shader _shader;
  std::unique_ptr<IndexedVertexArray> _mesh;
  glm::mat4 _modelMatrix;
};