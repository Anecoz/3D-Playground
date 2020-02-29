#include "DisimApplication.h"

#include "../Utils/GraphicsUtils.h"

#include <GLFW/glfw3.h>

#include <stdio.h>
#include <iostream>

DisimApplication::DisimApplication()
  : _camera(glm::vec3(100.0, 5.0, 100.0))
  , _gridGenerator(8)
{
  //std::cout << "Setting wireframe" << std::endl;
  //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

void DisimApplication::render()
{
  _cube.draw(_camera);
  /*for (auto grid: _currentGrids) {
    grid->draw(_camera);
  }*/
}

void DisimApplication::update(double delta)
{
  _camera.update(delta);

  std::vector<Grid*> activeGrids;
  _gridGenerator.update(_camera, delta, activeGrids);

  _currentGrids = std::move(activeGrids);

  /*for (auto grid: _currentGrids) {
    _terrainCollisionHandler.update(*grid, _camera, delta);
  }*/
}