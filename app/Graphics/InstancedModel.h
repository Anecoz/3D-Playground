#pragma once

#include "../Utils/ObjModelCache.h"
#include "Lowlevel/Shader.h"
#include "Box3D.h"

#include <glm/glm.hpp>

#include <cstddef>
#include <memory>
#include <string>

class Camera;
class IndexedVertexArray;

class InstancedModel
{
public:
  InstancedModel(CachedModelType type);
  ~InstancedModel();

  void draw(const Camera& camera);
  void drawShadowPass(const Camera& shadowCamera);

  void setInstanceMatrices(std::vector<glm::mat4>&& offsets);
  void setNumInstances(std::size_t numInstances);
  void setRotation(float xDeg, float yDeg, float zDeg);
  void setCenter(const glm::vec3& center);
  void setBoundingBox(Box3D&& box);

  const glm::vec3& getCenter();
  const Box3D& getBoundingBox();

private:
  Box3D _boundingBox;
  glm::vec3 _center;
  std::size_t _numInstances;
  Shader _shader;
  Shader _shadowShader;
  std::unique_ptr<IndexedVertexArray> _mesh;
  glm::mat4 _modelMatrix;
};