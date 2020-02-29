#pragma once

class Camera;
class Grid;

class TerrainCollisionHandler
{
public: 
  TerrainCollisionHandler();
  ~TerrainCollisionHandler() = default;

  void update(const Grid& grid, Camera& camera, double delta);

private:
  double _lastMovement;
  double _lastHeight;
};