#pragma once

#include "Lowlevel/Shader.h"

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
    std::vector<std::unique_ptr<InstancedModel>>&& models);
  ~Grid();

  void draw(const Camera& camera);

  bool heightAt(double x, double z, double* heightOut) const;
  const glm::vec3& getPosition();
  std::size_t getSize() const;

private:
  float* _verts;
  unsigned* _indices;

  bool _containsWater;

  glm::vec3 _position;
  std::size_t _size;

  glm::mat4 _modelMatrix;
  std::unique_ptr<IndexedVertexArray> _mesh;
  std::unique_ptr<IndexedVertexArray> _waterMesh;
  Shader _shader;
  Shader _waterShader;

  std::vector<std::unique_ptr<InstancedModel>> _models;
};