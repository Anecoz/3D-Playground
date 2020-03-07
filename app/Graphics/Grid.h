#pragma once

#include "Lowlevel/Texture.h"
#include "Box3D.h"

#include <glm/glm.hpp>

#include <cstddef>
#include <memory>
#include <vector>

class IndexedVertexArray;
class InstancedModel;
class Camera;

class Grid
{
public:
  Grid();
  Grid(
    float* verts,
    unsigned* indices,
    float* waterVerts,
    unsigned* waterIndices,
    bool containsWater,
    std::size_t size,
    const glm::vec3& position,
    const Box3D& boundingBox,
    std::vector<std::unique_ptr<InstancedModel>>&& models);
  ~Grid();

  void draw(const Camera& camera, const Texture& shadowMap, const glm::mat4& shadowMatrix);
  void drawShadowPass(const Camera& shadowCamera);
  void drawWater(const Camera& camera);

  const Box3D& getBoundingBox();

  bool heightAt(double x, double z, double* heightOut) const;
  const glm::vec3& getPosition();
  std::size_t getSize() const;

private:
  float* _verts;
  unsigned* _indices;

  bool _containsWater;

  glm::vec3 _position;
  std::size_t _size;
  Box3D _boundingBox;

  glm::mat4 _modelMatrix;
  std::unique_ptr<IndexedVertexArray> _mesh;
  std::unique_ptr<IndexedVertexArray> _waterMesh;

  std::vector<std::unique_ptr<InstancedModel>> _models;
};