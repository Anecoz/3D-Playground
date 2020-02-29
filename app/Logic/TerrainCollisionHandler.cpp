#include "TerrainCollisionHandler.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <stdio.h>
#include <cmath>

#include "../Graphics/Camera.h"
#include "../Graphics/Grid.h"
#include "../Utils/Math.h"

TerrainCollisionHandler::TerrainCollisionHandler()
  : _lastMovement(0.0)
  , _lastHeight(0.0)
{
}

void TerrainCollisionHandler::update(const Grid& grid, Camera& camera, double delta)
{
  auto camPos = camera.getPosition();
  double height;
  if (!grid.heightAt(camPos.x, camPos.z, &height)) {
    return;
  }

  height += 2.0;

  if (std::fabs(height - (double)camPos.y) < 0.0001) return;

  // If new height, reset timer
  if (std::fabs(height - _lastHeight) > 0.01) {
    _lastMovement = glfwGetTime();
  }
  // Update last height
  _lastHeight = height;

  // Lerp between cam height and height, using last movement as lerp param
  double now = glfwGetTime();
  double timeDelta = now - _lastMovement;
  double clampedFactor = math::clamp(timeDelta / 0.5, 0.0, 1.0);
  height = math::lerp(camPos.y, height, clampedFactor);
  camera.setPosition(glm::vec3(camPos.x, height, camPos.z));  
}