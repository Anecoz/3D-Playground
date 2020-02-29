#include "DisimApplication.h"
#include <GLFW/glfw3.h>

#include <stdio.h>
#include <iostream>

DisimApplication::DisimApplication()
  : _camera(glm::vec3(100.0, 5.0, 100.0))
{
  //std::cout << "Setting wireframe" << std::endl;
  //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  printf("Grid height at 100, 100: %lf\n", _grid.heightAt(100.0, 100.0));
}

void DisimApplication::render()
{
  //_cube.draw(_camera);
  _grid.draw(_camera);
}

void DisimApplication::update(double delta)
{
  _camera.update(delta);
  _terrainCollisionHandler.update(_grid, _camera, delta);
}