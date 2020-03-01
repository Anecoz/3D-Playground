#pragma once

#include <noise/noiseutils.h>
#include <cstddef>

class NoiseGenerator
{
public:
  NoiseGenerator(std::size_t mapSize, int xOffset, int yOffset);
  ~NoiseGenerator() = default;

  double getAt(int x, int z);

private:
  std::size_t _mapSize;

  noise::utils::NoiseMap _heightMap;

  int _xOffset;
  int _yOffset;
};