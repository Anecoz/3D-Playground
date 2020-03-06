#include "GridGenerator.h"

#include "../Graphics/Camera.h"
#include "../Graphics/InstancedModel.h"

#include <algorithm>
#include <chrono>
#include <limits>
#include <stdio.h>

#include <GL/glew.h>

#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/transform.hpp>
#include <glm/gtx/euler_angles.hpp>

#include <noise/noise.h>
#include <noise/noiseutils.h>

const double g_WaterHeight = -18.0;
const std::size_t g_ExtraGrids = 3;

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

  for (int xIdx = camxIdx - g_ExtraGrids; xIdx <= camxIdx + g_ExtraGrids; ++xIdx) {
    for (int zIdx = camzIdx - g_ExtraGrids; zIdx <= camzIdx + g_ExtraGrids; ++zIdx) {
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
            //printf("Could not find worker for grid at %d %d, guess I'll retry next update\n", xIdx, zIdx);
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
      int gridXIdx = static_cast<int>(result->_gridPosition.x) / _gridSize;
      int gridZIdx = static_cast<int>(result->_gridPosition.z) / _gridSize;

      // Remove from currently generating
      for (auto it = _currentlyGenerating.begin(); it != _currentlyGenerating.end(); ++it) {
        if (it->_x == gridXIdx && it->_z == gridZIdx) {
          _currentlyGenerating.erase(it);
          break;
        }
      }

      // Create the models
      std::vector<std::unique_ptr<InstancedModel>> models;
      for (auto& decorationData : result->_decorationData) {
        // Sanity check
        if (decorationData._type == DecorationType::Nothing) continue;

        InstancedModel* model = nullptr;
        if (decorationData._type == DecorationType::Tree) {
          model = new InstancedModel(CachedModelType::Tree);          
        }
        if (decorationData._type == DecorationType::Tree2) {
          model = new InstancedModel(CachedModelType::Tree2);          
        }
        if (decorationData._type == DecorationType::SmallRock) {
          model = new InstancedModel(CachedModelType::SmallRock);          
        }
        if (decorationData._type == DecorationType::Grass) {
          model = new InstancedModel(CachedModelType::Grass);          
        }
        if (decorationData._type == DecorationType::Rock) {
          model = new InstancedModel(CachedModelType::Rock);          
        }

        if (model) {
          model->setBoundingBox(std::move(decorationData._boundingBox));
          model->setNumInstances(decorationData._matrices.size());
          model->setCenter(decorationData._center);
          model->setInstanceMatrices(std::move(decorationData._matrices));
          models.emplace_back(std::unique_ptr<InstancedModel>(model));
        }
        else {
          printf("Unhandled Decoration type in generator!\n");
        } 
      }
      // Create the new Grid
      _grids.emplace_back(
        gridXIdx,
        gridZIdx,
        new Grid(
          result->_vertices,
          result->_indices,
          result->_waterVertices,
          result->_waterIndices,
          result->_containsWater,
          result->_gridSize,
          result->_gridPosition,
          result->_boundingBox,
          std::move(models)));
      printf("Grid at %d %d is done\n", gridXIdx, gridZIdx);
    }
  }

  // Any grids to unload?
  for (auto it = _grids.begin(); it != _grids.end();) {
    if (it->_index._x > camxIdx + g_ExtraGrids ||
        it->_index._x < camxIdx - g_ExtraGrids ||
        it->_index._z > camzIdx + g_ExtraGrids ||
        it->_index._z < camzIdx - g_ExtraGrids) {
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

void GridGenerator::GridData::addDecorationData(DecorationType type, glm::mat4&& matrix, glm::vec3&& position)
{
  auto it = std::find_if(_decorationData.begin(), _decorationData.end(), [type](const GridDecorationData& data) {
    return data._type == type;
  });

  if (it != _decorationData.end()) {
    // Already have decoration data of this type, add it to the list
    it->_matrices.emplace_back(std::move(matrix));
    it->_boundingBox.extend(position);
    it->_center += std::move(position);
    it->_center /= it->_matrices.size();    
  }
  else {
    // Create new decoration data of this type
    GridDecorationData data;
    data._type = type;
    data._boundingBox.extend(position);
    data._matrices.emplace_back(std::move(matrix));
    data._center = std::move(position);
    _decorationData.emplace_back(std::move(data));
  }
}

GridGenerator::GridData* GridGenerator::Worker::generateData(std::size_t size, const glm::vec3& posOffset)
{
  NoiseGenerator generator(size, (int)posOffset.x, (int)posOffset.z);

  unsigned numVerts = (size + 1) * (size + 1) * 3;
  unsigned numIndices = size * size * 6;

  // Terrain
  GridData* data = new GridData;
  data->_vertices = new float[numVerts];
  data->_indices = new unsigned[numIndices];
  data->_gridSize = size;
  data->_gridPosition = posOffset;

  // Water
  data->_waterVertices = new float[numVerts];
  data->_waterIndices = new unsigned[numIndices];
  data->_containsWater = false; // Until proven otherwise

  double minHeight = std::numeric_limits<double>::max();
  double maxHeight = std::numeric_limits<double>::min();
  for (unsigned y = 0; y <= size; ++y) {
    for (unsigned x = 0; x <= size; ++x) {
      // Terrain
      double dx = static_cast<double>(x);
      double dy = static_cast<double>(y);
      double height = generator.getHeightAt(x, y);

      if (height < minHeight) {
        minHeight = height;
      }
      if (height > maxHeight) {
        maxHeight = height;
      }

      if (height < g_WaterHeight) {
        data->_containsWater = true;
      }

      unsigned offset = 3 * (y * (size + 1));
      data->_vertices[offset + (x * 3) + 0] = (GLfloat)x;
      data->_vertices[offset + (x * 3) + 1] = height;
      data->_vertices[offset + (x * 3) + 2] = (GLfloat)y;

      // Water
      data->_waterVertices[offset + (x * 3) + 0] = (GLfloat)x;
      data->_waterVertices[offset + (x * 3) + 1] = g_WaterHeight;
      data->_waterVertices[offset + (x * 3) + 2] = (GLfloat)y;

      // Decorations
      DecorationData decData = generator.getDecorationDataAt(x, y);
      if (decData._type != DecorationType::Nothing) {
        double height = generator.getHeightAt(x + decData._offsetX, y + decData._offsetZ);

        // getHeightAt will return a huge number if we're outside of map
        if (height < 9000000.0 /*&& height >= -15.0 && height <= 10.0*/) {
          glm::vec3 pos(dx + decData._offsetX, height, dy + decData._offsetZ);
          pos += posOffset;
          glm::mat4 translation = glm::translate(pos);
          glm::mat4 scale = glm::scale(glm::vec3(decData._scale));
          glm::mat4 rotation = glm::yawPitchRoll(
            glm::radians(decData._yRotDeg),
            glm::radians(decData._xRotDeg),
            glm::radians(decData._zRotDeg));
          data->addDecorationData(decData._type, translation * rotation * scale, std::move(pos));
        }       
      }
    }
  }

  for (unsigned y = 0; y < size; ++y) {
    for (unsigned x = 0; x < size; ++x) {
      // Terrain
      data->_indices[6 * size * y + (x * 6) + 0] = y * (size + 1) + x;
      data->_indices[6 * size * y + (x * 6) + 1] = (y + 1) * (size + 1) + (x + 1);
      data->_indices[6 * size * y + (x * 6) + 2] = y * (size + 1) + (x + 1);
      data->_indices[6 * size * y + (x * 6) + 3] = y * (size + 1) + x;
      data->_indices[6 * size * y + (x * 6) + 4] = (y + 1) * (size + 1) + x;
      data->_indices[6 * size * y + (x * 6) + 5] = (y + 1) * (size + 1) + (x + 1);

      // Water
      data->_waterIndices[6 * size * y + (x * 6) + 0] = y * (size + 1) + x;
      data->_waterIndices[6 * size * y + (x * 6) + 1] = (y + 1) * (size + 1) + (x + 1);
      data->_waterIndices[6 * size * y + (x * 6) + 2] = y * (size + 1) + (x + 1);
      data->_waterIndices[6 * size * y + (x * 6) + 3] = y * (size + 1) + x;
      data->_waterIndices[6 * size * y + (x * 6) + 4] = (y + 1) * (size + 1) + x;
      data->_waterIndices[6 * size * y + (x * 6) + 5] = (y + 1) * (size + 1) + (x + 1);
    }
  }

  if (!data->_containsWater) {
    delete[] data->_waterVertices;
    delete[] data->_waterIndices;
  }

  glm::vec3 c0(data->_gridPosition.x, minHeight, data->_gridPosition.z);
  glm::vec3 c1(data->_gridPosition.x + data->_gridSize, maxHeight, data->_gridPosition.z + data->_gridSize);
  data->_boundingBox = Box3D(c0, c1);

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