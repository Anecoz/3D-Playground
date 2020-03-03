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
  Grid(float* verts, unsigned* indices, std::size_t size, const glm::vec3& position, std::vector<std::unique_ptr<InstancedModel>>&& models);
  ~Grid();

  void draw(const Camera& camera);

  bool heightAt(double x, double z, double* heightOut) const;
  const glm::vec3& getPosition();
  std::size_t getSize() const;

private:
  float* _verts;
  unsigned* _indices;

  glm::vec3 _position;
  std::size_t _size;

  glm::mat4 _modelMatrix;
  std::unique_ptr<IndexedVertexArray> _mesh;
  Shader _shader;

  std::vector<std::unique_ptr<InstancedModel>> _models;
};