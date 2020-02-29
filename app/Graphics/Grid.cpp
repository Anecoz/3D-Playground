#include "Grid.h"

#include "../PerlinNoise.hpp"

#include <GLFW/glfw3.h>
#include <stdio.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/transform.hpp>

#include "../Utils/GraphicsUtils.h"

Grid::Grid()
  : _position(0.0, 0.0, 0.0)
  , _size(512)
  , _modelMatrix(glm::scale(glm::vec3(1.0)))
  //, _mesh(GraphicsUtils::loadObj("/home/christoph/dev/rpg/app/grid.model"))
  //, _mesh(GraphicsUtils::createGrid(_size, _size))
  , _mesh(nullptr)
  /*, _shader(
    "/home/christoph/dev/rpg/app/Graphics/standard.vert",
    "/home/christoph/dev/rpg/app/Graphics/standard.tesc",
    "/home/christoph/dev/rpg/app/Graphics/standard.tese",
    "/home/christoph/dev/rpg/app/Graphics/standard.frag")*/
  , _shader(
    "/home/christoph/dev/rpg/app/Graphics/grid.vert",
    "/home/christoph/dev/rpg/app/Graphics/grid.frag")
{
  createMesh();
}

Grid::~Grid()
{
  _mesh = nullptr;
  delete[] _verts;
  delete[] _normals;
  delete[] _indices;
}

void Grid::createMesh()
{
  double start = glfwGetTime();

  const siv::PerlinNoise perlin(129832);
  const double frequency = 2.0;
  const double amplitude = 74.0;
  const int octaves = 8;
  const double fx = (double)_size / frequency;
  const double fy = (double)_size / frequency;

  unsigned numVerts = (_size + 1) * (_size + 1) * 3;
  unsigned numNormals = (_size + 1) * (_size + 1) * 3;
  unsigned numIndices = _size * _size * 6;

  GLfloat* verts = new GLfloat[numVerts];
  GLfloat* normals = new GLfloat[numNormals];
  unsigned* indices = new unsigned[numIndices];

  for (unsigned y = 0; y <= _size; ++y) {
    for (unsigned x = 0; x <= _size; ++x) {
      double dx = static_cast<double>(x);
      double dy = static_cast<double>(y);
      double height = amplitude * (GLfloat)perlin.accumulatedOctaveNoise2D_0_1(dx/fx, dy/fy, octaves);

      unsigned offset = 3 * (y * (_size + 1));
      verts[offset + (x * 3) + 0] = (GLfloat)x;
      verts[offset + (x * 3) + 1] = height;
      verts[offset + (x * 3) + 2] = (GLfloat)y;

      double offsetX = dx + 0.01;
      double offsetY = dy + 0.01;
      double dHeightX = amplitude * (GLfloat)perlin.accumulatedOctaveNoise2D_0_1(offsetX/fx, dy/fy, octaves);
      double dHeightY = amplitude * (GLfloat)perlin.accumulatedOctaveNoise2D_0_1(dx/fx, offsetY/fy, octaves);

      glm::vec3 p1(dx, height, dy);
      glm::vec3 p2(offsetX, dHeightX, dy);
      glm::vec3 p3(dx, dHeightY, offsetY);

      glm::vec3 ax1 = p2 - p1;
      glm::vec3 ax2 = p3 - p1;
      glm::vec3 norm = glm::normalize(glm::cross(ax1, ax2)); 

      normals[3 * (y * (_size + 1)) + (x * 3) + 0] = norm.x;
      normals[3 * (y * (_size + 1)) + (x * 3) + 1] = norm.y;
      normals[3 * (y * (_size + 1)) + (x * 3) + 2] = norm.z;
    }
  }

  for (unsigned y = 0; y < _size; ++y) {
    for (unsigned x = 0; x < _size; ++x) {
      indices[6 * _size * y + (x * 6) + 0] = y * (_size + 1) + x;
      indices[6 * _size * y + (x * 6) + 1] = (y + 1) * (_size + 1) + (x + 1);
      indices[6 * _size * y + (x * 6) + 2] = y * (_size + 1) + (x + 1);
      indices[6 * _size * y + (x * 6) + 3] = y * (_size + 1) + x;
      indices[6 * _size * y + (x * 6) + 4] = (y + 1) * (_size + 1) + x;
      indices[6 * _size * y + (x * 6) + 5] = (y + 1) * (_size + 1) + (x + 1);
    }
  }

  _verts = verts;
  _normals = normals;
  _indices = indices;

  _mesh = std::make_unique<IndexedVertexArray>(verts, indices, normals, numNormals, numVerts, numIndices, 3);
  double end = glfwGetTime();
  printf("createMesh took %lf ms\n", (end - start) * 1000.0);
}

double Grid::heightAt(double x, double z) const
{
  if (!_mesh) return 0.0;

  x = x - _position.x;
  z = z - _position.z;

  if (x < 0.0 || x > _size || z < 0.0 || z > _size) {
    printf("Cannot get height at %lf, %lf, outside range\n", x, z);
    return 0.0;
  }

  return _position.y + static_cast<double>(_verts[3 * (_size + 1) * (int)z + 3 * (int)x + 1]);
}

const glm::vec3& Grid::getPosition()
{
  return _position;
}

void Grid::draw(const Camera& camera)
{
  _modelMatrix = glm::translate(_position);

  _shader.bind();
  _shader.uploadVec(camera.getPosition(), "cameraPos");
  _shader.uploadMatrix(camera.getCamMatrix(), "camMatrix");
  _shader.uploadMatrix(camera.getProjection(), "projMatrix");
  _shader.uploadMatrix(_modelMatrix, "modelMatrix");
  //_shader.uploadVec(camera.getPosition(), "cameraPos");
  //_mesh->drawTesselated();
  _mesh->draw();
  _shader.unbind();
}
