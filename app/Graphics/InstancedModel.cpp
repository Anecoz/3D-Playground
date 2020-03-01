#include "InstancedModel.h"

#include <stdio.h>

#include "Camera.h"
#include "Lowlevel/IndexedVertexArray.h"
#include "../Utils/OBJLoader.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/transform.hpp>

InstancedModel::InstancedModel(const std::string& objPath)
  : _shader(
      "/home/christoph/dev/3D-Playground/app/Graphics/instancedmodel.vert",
      "/home/christoph/dev/3D-Playground/app/Graphics/instancedmodel.frag")
  , _mesh(nullptr)
  , _modelMatrix(glm::scale(glm::vec3(0.1)))
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

void InstancedModel::draw(const Camera& camera)
{
  if (!_mesh) {
    printf("InstancedModel: No mesh, not drawing\n");
    return;
  }

  // Test drawing 10000 models. Calculate offsets in x and z dir.
  std::vector<float> offsets;
  offsets.resize(10000 * 3);
  for (int x = 0; x < 100; ++x) {
    for (int z = 0; z < 100; ++z) {
      offsets[3 * 100 * z + 3 * x + 0] = 100.0f * (float)x;
      offsets[3 * 100 * z + 3 * x + 1] = 0.0f;
      offsets[3 * 100 * z + 3 * x + 2] = 100.0f * (float)z;
    }
  }

  _mesh->updateInstancePosOffsets(offsets);

  _shader.bind();
  _shader.uploadMatrix(_modelMatrix, "modelMatrix");
  _shader.uploadMatrix(camera.getCamMatrix(), "camMatrix");
  _shader.uploadMatrix(camera.getProjection(), "projMatrix");
  _shader.uploadVec(camera.getPosition(), "cameraPos");

  // draw instanced here
  _mesh->drawInstanced(10000);

  _shader.unbind();
}