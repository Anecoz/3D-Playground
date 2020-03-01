#pragma once

#include <noise/noiseutils.h>
#include <cstddef>

enum class DecorationType
{
  Nothing,
  Grass,
  Rock,
  Tree
};

class DecorationNoiseGenerator
{
public:
  DecorationNoiseGenerator(std::size_t mapSize, int xOffset, int yOffset);
  ~DecorationNoiseGenerator() = default;

  DecorationType getTypeAt(int x, int y);
  bool kill();
  double randomOffset(DecorationType type);
  double randomRotation(int min, int max); // degrees

private:
  noise::utils::NoiseMap _heightMap;

  std::size_t _mapSize;
  int _xOffset;
  int _yOffset;
};