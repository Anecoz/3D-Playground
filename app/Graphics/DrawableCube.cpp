#include "DrawableCube.h"

#include "Camera.h"

#include "../Utils/OBJLoader.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/transform.hpp>

DrawableCube::DrawableCube()
  : _shader("/home/christoph/dev/3D-Playground/app/Graphics/cube.vert", "/home/christoph/dev/3D-Playground/app/Graphics/cube.frag")
  , _modelMatrix(glm::scale(glm::vec3(1.0)))
{
  OBJData data;
  OBJLoader::loadFromFile(
  "/home/christoph/dev/3D-Playground/app/assets/low_poly_grass.obj",
  "/home/christoph/dev/3D-Playground/app/assets/",
  data);

  _cubeMesh.emplace_back(std::unique_ptr<IndexedVertexArray>(new IndexedVertexArray(
      (GLfloat*)&data._vertices[0],
      (GLuint*)&data._indices[0],
      (GLfloat*)&data._normals[0],
      (GLfloat*)&data._colors[0],
      (GLint)data._colors.size(),
      (GLint)data._normals.size(),
      (GLint)data._vertices.size(),
      (GLint)data._indices.size(),
      (GLint)3)));
}

void DrawableCube::draw(const Camera& camera)
{
  _shader.bind();
  _shader.uploadMatrix(_modelMatrix, "modelMatrix");
  _shader.uploadMatrix(camera.getCamMatrix(), "camMatrix");
  _shader.uploadMatrix(camera.getProjection(), "projMatrix");
  _shader.uploadVec(camera.getPosition(), "cameraPos");
  for (auto& mesh: _cubeMesh) {
    mesh->draw();
  }
  _shader.unbind();
}