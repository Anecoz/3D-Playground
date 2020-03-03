#include "DecorationGenerator.h"

#include "DecorationNoiseGenerator.h"
#include "../Graphics/Camera.h"
#include "../Graphics/Grid.h"
#include "../Graphics/InstancedModel.h"

#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/transform.hpp>
#include <glm/gtx/euler_angles.hpp>

#include <algorithm>

void DecorationGenerator::update(const Camera& camera, const std::vector<Grid*>& activeGrids, std::vector<InstancedModel*>& activeModelsOut)
{
  // TODO: Generate only close to camera, and use libnoise to figure out what and how much to place at diff locations
  for (auto& grid: activeGrids) {
    int gridX = static_cast<int>(grid->getPosition().x);
    int gridZ = static_cast<int>(grid->getPosition().z);

    auto found = std::find_if(_gridModels.begin(), _gridModels.end(), [gridX, gridZ](const GridEntry& entry){
      return entry._index._x == gridX && entry._index._z == gridZ;
    });

    if (found != _gridModels.end()) {
      // We have done this grid already, skip it.
      continue;
    }

    // Generate for this grid. Loop through and randomly place decorations.
    DecorationNoiseGenerator generator(grid->getSize(), gridX, gridZ);
    std::vector<glm::mat4> grassMatrices;
    std::vector<glm::mat4> rockMatrices;
    std::vector<glm::mat4> treeMatrices;
    glm::vec3 grassCenter(0.0, 0.0, 0.0);
    glm::vec3 rockCenter(0.0, 0.0, 0.0);
    glm::vec3 treeCenter(0.0, 0.0, 0.0);
    for (int x = 0; x < grid->getSize(); ++x/*x = x + 16*/) {
      for (int z = 0; z < grid->getSize(); ++z/*z = z + 16*/) {
        double dx = (double)x + grid->getPosition().x;
        double dz = (double)z + grid->getPosition().z;

        auto type = generator.getTypeAt(x, z);
        if (type == DecorationType::Nothing) {
          continue;
        }

        // Kill?
        if (generator.kill()) {
          continue;
        }

        double randomOffsetX = generator.randomOffset(type);
        double randomOffsetZ = generator.randomOffset(type);

        dx += randomOffsetX;
        dz += randomOffsetZ;

        double height;
        if (!grid->heightAt(dx, dz, &height)) {
          continue;
        }

        // Get some random rotation
        double xRot = generator.randomRotation(-10, 10);
        double yRot = generator.randomRotation(0, 360);
        double zRot = generator.randomRotation(-10, 10);
        glm::vec3 pos((float)dx, (float)height, (float)dz);
        glm::mat4 translation = glm::translate(pos);
        glm::mat4 rotation = glm::yawPitchRoll(glm::radians(yRot), glm::radians(xRot), glm::radians(zRot));

        glm::mat4 matrix = translation * rotation;
        if (type == DecorationType::Grass) {          
          grassMatrices.emplace_back(matrix);
          grassCenter += pos;
        }
        else if (type == DecorationType::Rock) {
          rockMatrices.emplace_back(matrix);
          rockCenter += pos;
        }
        else if (type == DecorationType::Tree) {
          treeMatrices.emplace_back(matrix);
          treeCenter += pos;
        }
      }
    }

    std::vector<InstancedModel*> models;
    if (!grassMatrices.empty()) {
      InstancedModel* model = new InstancedModel(CachedModelType::Grass);
      model->setNumInstances(grassMatrices.size());
      grassCenter /= grassMatrices.size();
      model->setCenter(grassCenter);
      model->setInstanceMatrices(std::move(grassMatrices));
      models.emplace_back(model);
    }
    if (!rockMatrices.empty()) {
      InstancedModel* model = new InstancedModel(CachedModelType::Rock);
      model->setNumInstances(rockMatrices.size());
      rockCenter /= rockMatrices.size();
      model->setCenter(rockCenter);
      model->setInstanceMatrices(std::move(rockMatrices));
      models.emplace_back(model);
    }
    if (!treeMatrices.empty()) {
      InstancedModel* model = new InstancedModel(CachedModelType::Tree);
      model->setNumInstances(treeMatrices.size());
      treeCenter /= treeMatrices.size();
      model->setCenter(treeCenter);
      model->setInstanceMatrices(std::move(treeMatrices));
      model->setRotation(0.0f, 90.0f, 0.0f);
      models.emplace_back(model);
    }
    
    _gridModels.emplace_back(GridIndex(gridX, gridZ), models);
  }

  // Do we have to remove anything from the book keeping?
  for (auto it = _gridModels.begin(); it != _gridModels.end();) {
    // Is this index in activegrids?
    int xIdx = it->_index._x;
    int zIdx = it->_index._z;

    auto found = std::find_if(activeGrids.begin(), activeGrids.end(), [xIdx, zIdx](Grid* grid){
      int gridX = static_cast<int>(grid->getPosition().x);
      int gridZ = static_cast<int>(grid->getPosition().z);

      return xIdx == gridX && zIdx == gridZ;
    });

    if (found == activeGrids.end()) {
      for (auto& model: it->_models) {
        delete model;
      }
      it = _gridModels.erase(it);
    }
    else {
      ++it;
    }   
  }

  // Everything left is active
  for (auto& gridModel: _gridModels) {
    for (auto& model: gridModel._models) {
      activeModelsOut.emplace_back(model);
    }
  }
}