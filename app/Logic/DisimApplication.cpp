#include "DisimApplication.h"

#include "../Utils/GraphicsUtils.h"

#include <GLFW/glfw3.h>

#include <stdio.h>
#include <iostream>

DisimApplication::DisimApplication()
  //: _instancedModel("/home/christoph/dev/3D-Playground/app/assets/low_poly_tree.obj")
  : _camera(glm::vec3(100.0, 5.0, 100.0))
  , _gridGenerator(8)
{
  //std::cout << "Setting wireframe" << std::endl;
  //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

void DisimApplication::render()
{
  //_cube.draw(_camera);
  for (auto grid: _currentGrids) {
    grid->draw(_camera);
  }
  for (auto model: _currentModels) {
    model->draw(_camera);
  }
  //_instancedModel.draw(_camera);
}

void DisimApplication::update(double delta)
{
  _camera.update(delta);

  std::vector<Grid*> activeGrids;
  _gridGenerator.update(_camera, delta, activeGrids);

  std::vector<InstancedModel*> activeModels;
  _decorationGenerator.update(_camera, activeGrids, activeModels);

  _currentGrids = std::move(activeGrids);
  _currentModels = std::move(activeModels);

  /*for (auto grid: _currentGrids) {
    _terrainCollisionHandler.update(*grid, _camera, delta);
  }*/
}