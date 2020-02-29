#pragma once

#include "Application.h"
#include "TerrainCollisionHandler.h"
#include "GridGenerator.h"
#include "../Graphics/DrawableCube.h"
#include "../Graphics/Camera.h"
#include "../Graphics/Grid.h"

#include <vector>

class DisimApplication : public Application
{
public:
  DisimApplication();
  ~DisimApplication() = default;

  void render() override;
  void update(double delta) override;

private:
  DrawableCube _cube;
  std::vector<Grid*> _currentGrids;
  Camera _camera;
  TerrainCollisionHandler _terrainCollisionHandler;
  GridGenerator _gridGenerator;
};