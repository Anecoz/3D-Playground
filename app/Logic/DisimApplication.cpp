#include "DisimApplication.h"

#include "../Graphics/Box3D.h"
#include "../Input/KeyInput.h"
#include "../Utils/GraphicsUtils.h"
#include "../Utils/ObjModelCache.h"
#include "../Utils/ShaderCache.h"
#include "../imgui/imgui.h"

#include <GLFW/glfw3.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/transform.hpp>
#include <glm/gtx/euler_angles.hpp>

#include <stdio.h>
#include <iostream>

DisimApplication::DisimApplication()
  : _camera(glm::vec3(10000.0, 500.0, 10000.0), ProjectionType::Perspective)
  , _gridGenerator(10)
{
  //std::cout << "Setting wireframe" << std::endl;
  //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  fillCaches();

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
  double scaleFactor = 30.0 / 30.0;
  glm::mat4 scale = glm::scale(glm::vec3(scaleFactor, scaleFactor, scaleFactor));
  glm::mat4 rot = glm::yawPitchRoll(yRot, 0.0, 0.0);
  glm::mat4 translation = glm::translate(avg);

  _testModel = new InstancedModel(CachedModelType::Bridge);
  _testModel->setCenter(avg);
  _testModel->setNumInstances(1);
  _testModel->setInstanceMatrices({translation * rot * scale});
}

void DisimApplication::fillCaches()
{
  ObjModelCache::cacheModel(std::string(ASSET_PATH) + "low_poly_grass.obj", CachedModelType::Grass);
  ObjModelCache::cacheModel(std::string(ASSET_PATH) + "low_poly_rock.obj", CachedModelType::Rock);
  ObjModelCache::cacheModel(std::string(ASSET_PATH) + "low_poly_rock_2.obj", CachedModelType::SmallRock);
  ObjModelCache::cacheModel(std::string(ASSET_PATH) + "low_poly_tree.obj", CachedModelType::Tree);
  ObjModelCache::cacheModel(std::string(ASSET_PATH) + "low_poly_pine.obj", CachedModelType::Tree2);
  ObjModelCache::cacheModel(std::string(ASSET_PATH) + "low_poly_tree_3.obj", CachedModelType::Tree3);
  ObjModelCache::cacheModel(std::string(ASSET_PATH) + "low_poly_bridge.obj", CachedModelType::Bridge);
  ObjModelCache::cacheModel(std::string(ASSET_PATH) + "low_poly_fence.obj", CachedModelType::Fence);

  Shader gridShader(
    std::string(SHADER_PATH) + "grid.vert",
    std::string(SHADER_PATH) + "grid.geom",
    std::string(SHADER_PATH) + "grid.frag");
  Shader waterShader(
    std::string(SHADER_PATH) + "water.vert",
    std::string(SHADER_PATH) + "water.geom",
    std::string(SHADER_PATH) + "water.frag");
  Shader gridShadowShader(
    std::string(SHADER_PATH) + "shadow.vert",
    std::string(SHADER_PATH) + "shadow.frag");
  Shader instancedModelShader(
    std::string(SHADER_PATH) + "instancedmodel.vert",
    std::string(SHADER_PATH) + "instancedmodel.geom",
    std::string(SHADER_PATH) + "instancedmodel.frag");
  Shader instancedModelShadowShader(
    std::string(SHADER_PATH) + "modelShadow.vert",
    std::string(SHADER_PATH) + "shadow.frag");

  ShaderCache::addShader(std::move(gridShader), ShaderType::Grid);
  ShaderCache::addShader(std::move(waterShader), ShaderType::Water);
  ShaderCache::addShader(std::move(gridShadowShader), ShaderType::ShadowGrid);
  ShaderCache::addShader(std::move(instancedModelShader), ShaderType::InstancedModel);
  ShaderCache::addShader(std::move(instancedModelShadowShader), ShaderType::ShadowInstancedModel);
}

static glm::vec3 g_sunDirection = glm::normalize(glm::vec3(-0.7f, -0.7f, -0.7f));

void DisimApplication::render()
{
  // Shadow pass first
  const Camera& shadowCam = _shadowGenerator.prepareShadowPass(_camera, g_sunDirection);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  for (auto grid: _currentGrids) {
    //if (_camera.insideFrustum(grid->getBoundingBox())) {
      grid->drawShadowPass(shadowCam);
    //}
  }

  _shadowGenerator.endShadowPass();
  const Texture& shadowMap = _shadowGenerator.getShadowTexture();

  // Normal, post-shadow pass
  for (auto grid: _currentGrids) {
    if (_camera.insideFrustum(grid->getBoundingBox())) {
      grid->draw(_camera, shadowMap, shadowCam.getCombined(), g_sunDirection);
    }
  }

  // Draw water after everything else, so blending works
  for (auto grid: _currentGrids) {
    if (_camera.insideFrustum(grid->getBoundingBox())) {
      grid->drawWater(_camera);
    }
  }

  _testModel->draw(_camera, g_sunDirection);
}

void DisimApplication::update(double delta)
{
  if (KeyInput::isKeyClicked(GLFW_KEY_ESCAPE)) {
    _camera._enabled = !_camera._enabled;
  }

  _camera.update(delta);

  std::vector<Grid*> activeGrids;
  _gridGenerator.update(_camera, delta, activeGrids);

  _currentGrids = std::move(activeGrids);

  /*for (auto grid: _currentGrids) {
    _terrainCollisionHandler.update(*grid, _camera, delta);
  }*/

  // ImGui stuff
  {
    ImGui::Begin("Debug");
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    ImGui::Text("Camera pos %.1f, %.1f, %.1f", _camera.getPosition().x, _camera.getPosition().y, _camera.getPosition().z);
    ImGui::End();
  }
  {
    ImGui::Begin("Parameters");
    ImGui::SliderFloat("Sun X", &g_sunDirection.x, -1.0f, 0.0f);
    ImGui::SliderFloat("Sun Y", &g_sunDirection.y, -1.0f, 0.0f);
    ImGui::SliderFloat("Sun Z", &g_sunDirection.z, -1.0f, 0.0f);
    ImGui::End();
  }
}