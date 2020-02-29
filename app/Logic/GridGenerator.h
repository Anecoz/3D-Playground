#pragma once

#include "../Graphics/Grid.h"
#include "NoiseGenerator.h"

#include <glm/glm.hpp>

#include <future>
#include <cstddef>
#include <memory>
#include <map>
#include <vector>

class Camera;

class GridGenerator
{
public:
  GridGenerator();
  GridGenerator(std::size_t numWorkerThreads);
  ~GridGenerator();

  void update(const Camera& camera, double delta, std::vector<Grid*>& activeGrids);

private:
  struct GridData
  {
    float* _vertices;
    float* _normals;
    unsigned* _indices;
    std::size_t _size;
    glm::vec3 _position;
  };

  struct Worker
  {
    enum class State
    {
      Idle,
      Working,
      ResultReady
    } _state;

    Worker()
      : _state(State::Idle)
      , _pendingResult(nullptr)
    {}

    void startGenerate(std::size_t size, const glm::vec3& posOffset);
    void update();
    std::unique_ptr<GridData> takeResult();

  private:
    GridData* generateData(std::size_t size, const glm::vec3& posOffset);
    std::unique_ptr<GridData> _pendingResult;
    std::future<GridData*> _future;
  };

  std::size_t _numWorkers;
  std::vector<Worker> _workers;

  struct GridIndex
  {
    GridIndex()
      : _x(0)
      , _z(0)
    {}

    GridIndex(int x, int z)
      : _x(x)
      , _z(z)
    {}

    int _x;
    int _z;
  };

  struct GridEntry
  {
    GridEntry(int x, int z, Grid* grid)
      : _index(x, z)
      , _grid(grid)
    {}

    GridIndex _index;
    Grid* _grid;
  };

  const std::size_t _gridSize = 512;
  std::vector<GridEntry> _grids;
  std::vector<GridIndex> _currentlyGenerating;

  //NoiseGenerator _noiseGenerator;
};