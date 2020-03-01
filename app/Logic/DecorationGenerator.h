#pragma once

#include <vector>

class Grid;
class InstancedModel;
class Camera;

class DecorationGenerator
{
public:
  DecorationGenerator() = default;
  ~DecorationGenerator() = default;

  void update(const Camera& camera, const std::vector<Grid*>& activeGrids, std::vector<InstancedModel*>& activeModelsOut);

private:
  struct GridIndex
  {
    GridIndex(int x, int z)
      : _x(x)
      , _z(z)
    {}

    int _x;
    int _z;
  };

  struct GridEntry
  {
    GridEntry(GridIndex index, std::vector<InstancedModel*> models)
      : _index(index)
      , _models(models)
    {}

    GridIndex _index;
    std::vector<InstancedModel*> _models;
  };

  std::vector<GridEntry> _gridModels;
};