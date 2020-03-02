#include "Grid.h"

#include <GLFW/glfw3.h>
#include <stdio.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/transform.hpp>

#include "Camera.h"
#include "Lowlevel/IndexedVertexArray.h"
#include "../Utils/GraphicsUtils.h"

Grid::Grid()
  : _position(0.0, 0.0, 0.0)
  , _size(512)
  , _modelMatrix(glm::scale(glm::vec3(1.0)))
  , _mesh(nullptr)
  , _shader(
    "/home/christoph/dev/3D-Playground/app/Graphics/grid.vert",
    "/home/christoph/dev/3D-Playground/app/Graphics/grid.geom",
    "/home/christoph/dev/3D-Playground/app/Graphics/grid.frag")
{}

Grid::Grid(float* verts, float* normals, unsigned* indices, std::size_t size, const glm::vec3& position)
  : _verts(verts)
  , _normals(normals)
  , _indices(indices)
  , _position(position)
  , _size(size)
  , _modelMatrix(glm::scale(glm::vec3(1.0)))
  , _mesh(nullptr)
  , _shader(
    "/home/christoph/dev/3D-Playground/app/Graphics/grid.vert",
    "/home/christoph/dev/3D-Playground/app/Graphics/grid.geom",
    "/home/christoph/dev/3D-Playground/app/Graphics/grid.frag")
{
  unsigned numVerts = (_size + 1) * (_size + 1) * 3;
  unsigned numNormals = (_size + 1) * (_size + 1) * 3;
  unsigned numIndices = _size * _size * 6;

  _mesh = std::make_unique<IndexedVertexArray>(verts, indices, normals, numNormals, numVerts, numIndices, 3);
}

Grid::~Grid()
{
  _mesh = nullptr;
  delete[] _verts;
  delete[] _normals;
  delete[] _indices;
}

bool Grid::heightAt(double x, double z, double* heightOut) const
{
  if (!_mesh) return false;

  x = x - _position.x;
  z = z - _position.z;

  if (x < 0.0 || x > _size || z < 0.0 || z > _size) {
    //printf("Cannot get height at %lf, %lf, outside range\n", x, z);
    return false;
  }

  *heightOut = _position.y + static_cast<double>(_verts[3 * (_size + 1) * (int)z + 3 * (int)x + 1]);
  return true;
}

const glm::vec3& Grid::getPosition()
{
  return _position;
}

std::size_t Grid::getSize() const
{
  return _size;
}

void Grid::draw(const Camera& camera)
{
  _modelMatrix = glm::translate(_position);

  _shader.bind();
  _shader.uploadVec(camera.getPosition(), "cameraPos");
  _shader.uploadMatrix(camera.getCamMatrix(), "camMatrix");
  _shader.uploadMatrix(camera.getProjection(), "projMatrix");
  _shader.uploadMatrix(_modelMatrix, "modelMatrix");
  _mesh->draw();
  _shader.unbind();
}
