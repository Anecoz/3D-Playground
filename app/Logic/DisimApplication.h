#pragma once

#include "Application.h"
#include "TerrainCollisionHandler.h"
#include "../Graphics/DrawableCube.h"
#include "../Graphics/Camera.h"
#include "../Graphics/Grid.h"

class DisimApplication : public Application
{
public:
  DisimApplication();
  ~DisimApplication() = default;

  void render() override;
  void update(double delta) override;

private:
  DrawableCube _cube;
  Grid _grid;
  Camera _camera;
  TerrainCollisionHandler _terrainCollisionHandler;
};