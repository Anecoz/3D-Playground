#pragma once

#include "../Graphics/Grid.h"
#include "TerrainNoiseGenerator.h"

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
  struct GridDecorationData
  {
    DecorationType _type;
    std::vector<glm::mat4> _matrices; // Size of this vector also determines number of instances
    glm::vec3 _center;
    Box3D _boundingBox;
  };

  struct GridData
  {
    float* _vertices;
    unsigned* _indices;
    std::size_t _gridSize;
    glm::vec3 _gridPosition;
    Box3D _boundingBox;

    bool _containsWater;
    float* _waterVertices;
    unsigned* _waterIndices;
    std::size_t _numWaterVerts;

    std::vector<GridDecorationData> _decorationData;

    void addDecorationData(DecorationType type, glm::mat4&& matrix, glm::vec3&& position);
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

  const double _waterHeight = -15.0;
  const std::size_t _gridSize = 256;
  std::vector<GridEntry> _grids;
  std::vector<GridIndex> _currentlyGenerating;

  //NoiseGenerator _noiseGenerator;
};