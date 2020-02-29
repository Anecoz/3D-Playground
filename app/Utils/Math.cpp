#include "Math.h"

namespace math
{

double clamp(double val, double min, double max)
{
  if (val > max) {
    return max;
  }
  if (val < min) {
    return min;
  }

  return val;
}

double lerp(double a, double b, double factor)
{
  return a * (1.0 - factor) + b * factor;
}

}
