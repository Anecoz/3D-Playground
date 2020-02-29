#include "DrawableCube.h"

#include "Camera.h"

#include "../Utils/GraphicsUtils.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/transform.hpp>

DrawableCube::DrawableCube()
  : _shader("/home/christoph/dev/rpg/app/Graphics/cube.vert", "/home/christoph/dev/rpg/app/Graphics/cube.frag")
  , _cubeMesh(GraphicsUtils::createUnitCube())
  , _modelMatrix(glm::scale(glm::vec3(1.0)))
{
}

void DrawableCube::draw(const Camera& camera)
{
  _shader.bind();
  _shader.uploadMatrix(_modelMatrix, "modelMatrix");
  _shader.uploadMatrix(camera.getCamMatrix(), "camMatrix");
  _shader.uploadMatrix(camera.getProjection(), "projMatrix");
  _cubeMesh->draw();
  _shader.unbind();
}