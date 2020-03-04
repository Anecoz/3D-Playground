#include "Box3D.h"

Box3D::Box3D(const glm::vec3& min, const glm::vec3& max)
  : _min(min)
  , _max(max)
{}

glm::vec3 Box3D::getMin() const
{
  return _min;
}

glm::vec3 Box3D::getMax() const
{
  return _max;
}