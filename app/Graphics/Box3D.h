#pragma once

#include <glm/glm.hpp>

class Box3D
{
public:
  Box3D(const glm::vec3& min, const glm::vec3& max);

  glm::vec3 getMin() const;
  glm::vec3 getMax() const;

private:
  glm::vec3 _min;
  glm::vec3 _max;
};