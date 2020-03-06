#include "DisimApplication.h"

#include "../Graphics/Box3D.h"
#include "../Utils/GraphicsUtils.h"
#include "../Utils/ObjModelCache.h"

#include <GLFW/glfw3.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/transform.hpp>
#include <glm/gtx/euler_angles.hpp>

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
  ObjModelCache::cacheModel("/home/christoph/dev/3D-Playground/app/assets/low_poly_bridge.obj", CachedModelType::Bridge);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  // TESTING BRIDGE PLACEMENT
  glm::vec3 p1(9163.54, -13.7, 9036.75);
  glm::vec3 p2(9095.32, -9.1, 9104.97);
  glm::vec3 avg = p1 + p2;
  avg /= 2;
  glm::vec3 dir = p2 - p1;
  dir = glm::normalize(dir);
  double distance = glm::distance(p1, p2);

  double yRot = std::acos(glm::dot(glm::vec3(0.0, 0.0, 1.0), dir));

  // Bridge is 30m long
  double scaleFactor = distance / 30.0;
  glm::mat4 scale = glm::scale(glm::vec3(scaleFactor, scaleFactor, scaleFactor));
  glm::mat4 rot = glm::yawPitchRoll(yRot, 0.0, 0.0);
  glm::mat4 translation = glm::translate(avg);

  _testModel = new InstancedModel(CachedModelType::Bridge);
  _testModel->setCenter(avg);
  _testModel->setNumInstances(1);
  _testModel->setInstanceMatrices({translation * rot * scale});
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

  _testModel->draw(_camera);
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