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

struct DecorationData
{
  DecorationType _type;
  double _scale;
  double _xRotDeg;
  double _yRotDeg;
  double _zRotDeg;
};

class NoiseGenerator
{
public:
  NoiseGenerator(std::size_t mapSize, int xOffset, int yOffset);
  ~NoiseGenerator() = default;

  double getHeightAt(int x, int z);
  DecorationData getDecorationDataAt(int x, int z);

private:
  void buildHeightMap();
  void buildDecorationMap();

  std::size_t _mapSize;

  noise::utils::NoiseMap _heightMap;
  noise::utils::NoiseMap _decorationMap;

  double _scale;
  int _xOffset;
  int _yOffset;
};