#include "GridGenerator.h"

#include <algorithm>
#include <chrono>
#include <stdio.h>

#include <GL/glew.h>

#include "../Graphics/Camera.h"
#include "../PerlinNoise.hpp"

#include <noise/noise.h>
#include <noise/noiseutils.h>

GridGenerator::GridGenerator()
  : _numWorkers(4)
{
  _workers.reserve(_numWorkers);
  for (unsigned i = 0; i < _numWorkers; ++i) {
    _workers.emplace_back();
  }
}

GridGenerator::GridGenerator(std::size_t numWorkerThreads)
  : _numWorkers(numWorkerThreads)
{
  _workers.reserve(_numWorkers);
  for (unsigned i = 0; i < _numWorkers; ++i) {
    _workers.emplace_back();
  }
}

GridGenerator::~GridGenerator()
{
  // TODO: Join all workers?
}

void GridGenerator::update(const Camera& camera, double delta, std::vector<Grid*>& activeGrids)
{
  // Any new work to do?
  int camxIdx = static_cast<int>(camera.getPosition().x) / _gridSize;
  int camzIdx = static_cast<int>(camera.getPosition().z) / _gridSize;

  for (int xIdx = camxIdx - 1; xIdx <= camxIdx + 1; ++xIdx) {
    for (int zIdx = camzIdx - 1; zIdx <= camzIdx + 1; ++zIdx) {
      auto found = std::find_if(_grids.begin(), _grids.end(), [xIdx, zIdx](const GridEntry& entry) {
        return entry._index._x == xIdx && entry._index._z == zIdx;
      });
      if (found == _grids.end()) {
        // Check if we're currently generating this grid
        bool currGenerating = false;
        for (auto& generating: _currentlyGenerating) {
          if (generating._x == xIdx && generating._z == zIdx) {
            currGenerating = true;
            break;
          }
        }

        if (!currGenerating) {
          // Grid we're standing on is not loaded, start loading
          bool foundWorker = false;
          for (auto& worker: _workers) {
            if (worker._state == Worker::State::Idle) {
              worker.startGenerate(_gridSize, glm::vec3(
                static_cast<double>(xIdx) * static_cast<double>(_gridSize),
                0.0,
                static_cast<double>(zIdx) * static_cast<double>(_gridSize)));
              _currentlyGenerating.emplace_back(xIdx, zIdx);
              foundWorker = true;
              break;
            }
          }
          if (!foundWorker) {
            printf("Could not find worker for grid at %d %d, guess I'll retry next update\n", xIdx, zIdx);
          }
          else {
            printf("Worker started for grid %d %d\n", xIdx, zIdx);
          }
        }    
      }
    }
  }  

  // Any new grids that are done loading?
  for (auto& worker: _workers) {
    worker.update();

    if (worker._state == Worker::State::ResultReady) {
      auto result = worker.takeResult();
      int gridXIdx = static_cast<int>(result->_position.x) / _gridSize;
      int gridZIdx = static_cast<int>(result->_position.z) / _gridSize;

      // Remove from currently generating
      for (auto it = _currentlyGenerating.begin(); it != _currentlyGenerating.end(); ++it) {
        if (it->_x == gridXIdx && it->_z == gridZIdx) {
          _currentlyGenerating.erase(it);
          break;
        }
      }

      printf("Grid at %d %d is done\n", gridXIdx, gridZIdx);
      _grids.emplace_back(gridXIdx, gridZIdx, new Grid(result->_vertices, result->_normals, result->_indices, result->_size, result->_position));
    }
  }

  // Any grids to unload?
  for (auto it = _grids.begin(); it != _grids.end();) {
    if (it->_index._x > camxIdx + 1 ||
        it->_index._x < camxIdx - 1 ||
        it->_index._z > camzIdx + 1 ||
        it->_index._z < camzIdx - 1) {
      printf("Removing grid at %d %d\n", it->_index._x, it->_index._z);
      delete it->_grid;
      it = _grids.erase(it);
    }
    else {
      ++it;
    }
  }

  // All that is left in _grids now should be active
  for (auto& entry: _grids) {
    activeGrids.emplace_back(entry._grid);
  }
}

GridGenerator::GridData* GridGenerator::Worker::generateData(std::size_t size, const glm::vec3& posOffset)
{
  NoiseGenerator generator(size, (int)posOffset.x, (int)posOffset.z);

  unsigned numVerts = (size + 1) * (size + 1) * 3;
  unsigned numNormals = (size + 1) * (size + 1) * 3;
  unsigned numIndices = size * size * 6;

  GridData* data = new GridData;
  data->_vertices = new float[numVerts];
  data->_indices = new unsigned[numIndices];
  data->_normals = new float[numNormals];
  data->_size = size;
  data->_position = posOffset;

  for (unsigned y = 0; y <= size; ++y) {
    for (unsigned x = 0; x <= size; ++x) {
      double dx = static_cast<double>(x);
      double dy = static_cast<double>(y);
      double height = generator.getAt(x, y);

      unsigned offset = 3 * (y * (size + 1));
      data->_vertices[offset + (x * 3) + 0] = (GLfloat)x;
      data->_vertices[offset + (x * 3) + 1] = height;
      data->_vertices[offset + (x * 3) + 2] = (GLfloat)y;

      double offsetX = dx + 0.01;
      double offsetY = dy + 0.01;
      double dHeightX = generator.getAt(x + 1, y);
      double dHeightY = generator.getAt(x, y + 1);

      glm::vec3 p1(dx, height, dy);
      glm::vec3 p2(dx + 1.0, dHeightX, dy);
      glm::vec3 p3(dx, dHeightY, dy + 1.0);

      glm::vec3 ax1 = p2 - p1;
      glm::vec3 ax2 = p3 - p1;
      glm::vec3 norm = -glm::normalize(glm::cross(ax1, ax2)); 

      data->_normals[3 * (y * (size + 1)) + (x * 3) + 0] = norm.x;
      data->_normals[3 * (y * (size + 1)) + (x * 3) + 1] = norm.y;
      data->_normals[3 * (y * (size + 1)) + (x * 3) + 2] = norm.z;
    }
  }

  for (unsigned y = 0; y < size; ++y) {
    for (unsigned x = 0; x < size; ++x) {
      data->_indices[6 * size * y + (x * 6) + 0] = y * (size + 1) + x;
      data->_indices[6 * size * y + (x * 6) + 1] = (y + 1) * (size + 1) + (x + 1);
      data->_indices[6 * size * y + (x * 6) + 2] = y * (size + 1) + (x + 1);
      data->_indices[6 * size * y + (x * 6) + 3] = y * (size + 1) + x;
      data->_indices[6 * size * y + (x * 6) + 4] = (y + 1) * (size + 1) + x;
      data->_indices[6 * size * y + (x * 6) + 5] = (y + 1) * (size + 1) + (x + 1);
    }
  }

  return data;
}

void GridGenerator::Worker::startGenerate(std::size_t size, const glm::vec3& posOffset)
{
  if (_state != State::Idle) {
    printf("Grid worker told to start generate, but is not idle! I refuse!\n");
    return;
  }

  _future = std::async(std::launch::async, [this, size, posOffset]{
    return generateData(size, posOffset);
  });
  _state = State::Working;
}

void GridGenerator::Worker::update()
{
  if (_state == State::Working) {
    if (_future.wait_for(std::chrono::seconds(0)) == std::future_status::ready) {
      auto data = _future.get();

      _pendingResult.reset(data);
      _state = State::ResultReady;
    }
  }
}

std::unique_ptr<GridGenerator::GridData> GridGenerator::Worker::takeResult()
{
  if (_state == State::ResultReady) {
    auto ret = std::move(_pendingResult);
    _pendingResult = nullptr;
    _state = State::Idle;
    return std::move(ret);
  }
  return nullptr;
}