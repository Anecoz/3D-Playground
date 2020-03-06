#include "DisimApplication.h"

#include "../Graphics/Box3D.h"
#include "../Utils/GraphicsUtils.h"
#include "../Utils/ObjModelCache.h"

#include <GLFW/glfw3.h>

#include <stdio.h>
#include <iostream>

DisimApplication::DisimApplication()
  : _camera(glm::vec3(10000.0, 500.0, 10000.0))
  , _gridGenerator(10)
{
  //std::cout << "Setting wireframe" << std::endl;
  //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  ObjModelCache::cacheModel("/home/christoph/dev/3D-Playground/app/assets/low_poly_grass.obj", CachedModelType::Grass);
  ObjModelCache::cacheModel("/home/christoph/dev/3D-Playground/app/assets/low_poly_rock.obj", CachedModelType::Rock);
  ObjModelCache::cacheModel("/home/christoph/dev/3D-Playground/app/assets/low_poly_rock_2.obj", CachedModelType::SmallRock);
  ObjModelCache::cacheModel("/home/christoph/dev/3D-Playground/app/assets/low_poly_tree.obj", CachedModelType::Tree);
  ObjModelCache::cacheModel("/home/christoph/dev/3D-Playground/app/assets/low_poly_pine.obj", CachedModelType::Tree2);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  
}

void DisimApplication::render()
{
  for (auto grid: _currentGrids) {
    if (_camera.insideFrustum(grid->getBoundingBox())) {
      grid->draw(_camera);
    }
    /*else {
      printf("Skipping grid\n");
    }*/
  }

  // Draw water after everything else, so blending works
  for (auto grid: _currentGrids) {
    if (_camera.insideFrustum(grid->getBoundingBox())) {
      grid->drawWater(_camera);
    }
  }
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