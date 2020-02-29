#pragma once

#include "Lowlevel/IndexedVertexArray.h"
#include "Lowlevel/Shader.h"
#include "Camera.h"

#include <glm/glm.hpp>

#include <memory>

class Grid
{
public:
  Grid();
  ~Grid();

  void draw(const Camera& camera);

  double heightAt(double x, double z) const;
  const glm::vec3& getPosition();

private:
  void createMesh();

  float* _verts;
  float* _normals;
  unsigned* _indices;

  glm::vec3 _position;
  unsigned _size;

  glm::mat4 _modelMatrix;
  std::unique_ptr<IndexedVertexArray> _mesh;
  Shader _shader;
};