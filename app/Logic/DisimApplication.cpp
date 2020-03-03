#include "DisimApplication.h"

#include "../Utils/GraphicsUtils.h"
#include "../Utils/ObjModelCache.h"

#include <GLFW/glfw3.h>

#include <stdio.h>
#include <iostream>

DisimApplication::DisimApplication()
  //: _instancedModel("/home/christoph/dev/3D-Playground/app/assets/low_poly_tree.obj")
  : _camera(glm::vec3(10000.0, 500.0, 10000.0))
  , _gridGenerator(8)
{
  //std::cout << "Setting wireframe" << std::endl;
  //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  ObjModelCache::cacheModel("/home/christoph/dev/3D-Playground/app/assets/low_poly_grass.obj", CachedModelType::Grass);
  ObjModelCache::cacheModel("/home/christoph/dev/3D-Playground/app/assets/low_poly_rock.obj", CachedModelType::Rock);
  ObjModelCache::cacheModel("/home/christoph/dev/3D-Playground/app/assets/low_poly_tree.obj", CachedModelType::Tree);
}

void DisimApplication::render()
{
  //_cube.draw(_camera);
  for (auto grid: _currentGrids) {
    grid->draw(_camera);
  }
  for (auto model: _currentModels) {
    if (glm::distance(model->getCenter(), _camera.getPosition()) < 500.0f) {
      model->draw(_camera);
    }
  }
  //_instancedModel.draw(_camera);
}

void DisimApplication::update(double delta)
{
  _camera.update(delta);

  std::vector<Grid*> activeGrids;
  _gridGenerator.update(_camera, delta, activeGrids);

  std::vector<InstancedModel*> activeModels;
  //_decorationGenerator.update(_camera, activeGrids, activeModels);

  _currentGrids = std::move(activeGrids);
  _currentModels = std::move(activeModels);

  /*for (auto grid: _currentGrids) {
    _terrainCollisionHandler.update(*grid, _camera, delta);
  }*/
}