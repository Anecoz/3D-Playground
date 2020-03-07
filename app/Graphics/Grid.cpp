#include "Grid.h"

#include <GLFW/glfw3.h>
#include <stdio.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/transform.hpp>

#include "Camera.h"
#include "InstancedModel.h"
#include "Lowlevel/IndexedVertexArray.h"
#include "../Utils/GraphicsUtils.h"
#include "../Utils/ShaderCache.h"

Grid::Grid()
  : _position(0.0, 0.0, 0.0)
  , _size(512)
  , _modelMatrix(glm::scale(glm::vec3(1.0)))
  , _mesh(nullptr)
{}

Grid::Grid(
  float* verts,
  unsigned* indices,
  float* waterVerts,
  unsigned* waterIndices,
  bool containsWater,
  std::size_t size,
  const glm::vec3& position,
  const Box3D& boundingBox,
  std::vector<std::unique_ptr<InstancedModel>>&& models)
  : _verts(verts)
  , _indices(indices)
  , _containsWater(containsWater)
  , _position(position)
  , _size(size)
  , _boundingBox(boundingBox)
  , _modelMatrix(glm::scale(glm::vec3(1.0)))
  , _mesh(nullptr)
  , _models(std::move(models))
{
  unsigned numVerts = (_size + 1) * (_size + 1) * 3;
  unsigned numIndices = _size * _size * 6;

  _mesh = std::make_unique<IndexedVertexArray>(verts, indices, numVerts, numIndices, 3);

  if (containsWater) {
    _waterMesh = std::make_unique<IndexedVertexArray>(waterVerts, waterIndices, numVerts, numIndices, 3);

    delete[] waterVerts;
    delete[] waterIndices;
  }
}

Grid::~Grid()
{
  _mesh = nullptr;
  delete[] _verts;
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

const Box3D& Grid::getBoundingBox()
{
  return _boundingBox;
}

const glm::vec3& Grid::getPosition()
{
  return _position;
}

std::size_t Grid::getSize() const
{
  return _size;
}

void Grid::draw(const Camera& camera, const Texture& shadowMap, const glm::mat4& shadowMatrix)
{
  for (auto& model: _models) {
    if (camera.insideFrustum(model->getBoundingBox())) {
      model->draw(camera);
    }
  }

  shadowMap.bind();

  _modelMatrix = glm::translate(_position);

  const auto& shader = ShaderCache::getShader(ShaderType::Grid);
  shader.bind();
  shader.uploadVec(camera.getPosition(), "cameraPos");
  shader.uploadMatrix(camera.getCamMatrix(), "camMatrix");
  shader.uploadMatrix(camera.getProjection(), "projMatrix");
  shader.uploadMatrix(_modelMatrix, "modelMatrix");
  shader.uploadMatrix(shadowMatrix, "shadowMatrix");
  _mesh->draw();
  shader.unbind();
}

void Grid::drawShadowPass(const Camera& shadowCamera)
{
  for (auto& model: _models) {
    //if (camera.insideFrustum(model->getBoundingBox())) {
      model->drawShadowPass(shadowCamera);
    //}
  }

  _modelMatrix = glm::translate(_position);

  const auto& shadowShader = ShaderCache::getShader(ShaderType::ShadowGrid);
  shadowShader.bind();
  shadowShader.uploadVec(shadowCamera.getPosition(), "camPosition");
  shadowShader.uploadMatrix(shadowCamera.getCamMatrix(), "camMatrix");
  shadowShader.uploadMatrix(shadowCamera.getProjection(), "projMatrix");
  shadowShader.uploadMatrix(_modelMatrix, "modelMatrix");
  _mesh->draw();
  shadowShader.unbind();
}

void Grid::drawWater(const Camera& camera)
{
  _modelMatrix = glm::translate(_position);

  const auto& waterShader = ShaderCache::getShader(ShaderType::Water);
  if (_containsWater) {
    waterShader.bind();
    waterShader.uploadVec(camera.getPosition(), "cameraPos");
    waterShader.uploadMatrix(camera.getCamMatrix(), "camMatrix");
    waterShader.uploadMatrix(camera.getProjection(), "projMatrix");
    waterShader.uploadMatrix(_modelMatrix, "modelMatrix");
    waterShader.uploadFloat((float)glfwGetTime(), "time");
    _waterMesh->draw();
    waterShader.unbind();
  }
}
