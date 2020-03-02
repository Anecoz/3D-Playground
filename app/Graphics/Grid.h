#pragma once

#include "Lowlevel/Shader.h"

#include <glm/glm.hpp>

#include <cstddef>
#include <memory>

class IndexedVertexArray;
class Camera;

class Grid
{
public:
  Grid();
  Grid(float* verts, float* normals, unsigned* indices, std::size_t size, const glm::vec3& position);
  ~Grid();

  void draw(const Camera& camera);

  bool heightAt(double x, double z, double* heightOut) const;
  const glm::vec3& getPosition();
  std::size_t getSize() const;

private:
  float* _verts;
  float* _normals;
  unsigned* _indices;

  glm::vec3 _position;
  std::size_t _size;

  glm::mat4 _modelMatrix;
  std::unique_ptr<IndexedVertexArray> _mesh;
  Shader _shader;
};