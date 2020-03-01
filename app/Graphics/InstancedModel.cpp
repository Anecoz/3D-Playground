#include "InstancedModel.h"

#include <stdio.h>

#include "Camera.h"
#include "Lowlevel/IndexedVertexArray.h"
#include "../Utils/OBJLoader.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/transform.hpp>
#include <glm/gtx/euler_angles.hpp>

InstancedModel::InstancedModel(const std::string& objPath)
  : _numInstances(1)
  ,  _shader(
      "/home/christoph/dev/3D-Playground/app/Graphics/instancedmodel.vert",
      "/home/christoph/dev/3D-Playground/app/Graphics/instancedmodel.geom",
      "/home/christoph/dev/3D-Playground/app/Graphics/instancedmodel.frag")
  , _mesh(nullptr)
  , _modelMatrix(glm::scale(glm::vec3(1.0)))
{
  // Find directory
  std::string directory = objPath;
  const size_t lastSlashIdx = objPath.rfind('/');
  if (std::string::npos != lastSlashIdx) {
    directory = objPath.substr(0, lastSlashIdx);
  }

  OBJData data;
  if (OBJLoader::loadFromFile(objPath, directory, data)) {
    _mesh = std::unique_ptr<IndexedVertexArray>(new IndexedVertexArray(
      (GLfloat*)&data._vertices[0],
      (GLuint*)&data._indices[0],
      (GLfloat*)&data._normals[0],
      (GLfloat*)&data._colors[0],
      data._colors.size(),
      data._normals.size(),
      data._vertices.size(),
      data._indices.size(),
      3,
      true
    ));
  }
}

InstancedModel::~InstancedModel()
{
  _mesh = nullptr;
}

void InstancedModel::setInstanceMatrices(std::vector<glm::mat4>&& matrices)
{
  if (!_mesh) {
    printf("InstancedModel: No mesh, not updating matrices\n");
    return;
  }

  _mesh->updateInstanceMatrices(std::move(matrices));
}

void InstancedModel::setNumInstances(std::size_t numInstances)
{
  if (!_mesh) {
    printf("InstancedModel: No mesh, not updating num instances\n");
    return;
  }
  _numInstances = numInstances;
}

void InstancedModel::setRotation(float xDeg, float yDeg, float zDeg)
{
  _modelMatrix = glm::yawPitchRoll(yDeg, xDeg, zDeg);
}

void InstancedModel::draw(const Camera& camera)
{
  if (!_mesh) {
    printf("InstancedModel: No mesh, not drawing\n");
    return;
  }

  _shader.bind();
  _shader.uploadMatrix(_modelMatrix, "modelMatrix");
  _shader.uploadMatrix(camera.getCamMatrix(), "camMatrix");
  _shader.uploadMatrix(camera.getProjection(), "projMatrix");
  _shader.uploadVec(camera.getPosition(), "cameraPos");

  _mesh->drawInstanced(_numInstances);

  _shader.unbind();
}