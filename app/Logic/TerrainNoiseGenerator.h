#pragma once

#include <noise/noiseutils.h>
#include <cstddef>

enum class DecorationType
{
  Nothing,
  Grass,
  Rock,
  SmallRock,
  Tree,
  Tree2,
  Tree3,
  Fence
};

struct DecorationData
{
  DecorationType _type;
  double _scale;
  double _offsetX;
  double _offsetZ;
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
  void buildDecorationScaleMap();
  void buildDecorationOffsetMaps();
  void buildDecorationRotMaps();
  void buildRoadMaps();

  const std::size_t _mapBufferSize = 16;;
  std::size_t _mapSize;

  noise::utils::NoiseMap _heightMap;
  noise::utils::NoiseMap _decorationMap;
  noise::utils::NoiseMap _decorationScaleMap;
  noise::utils::NoiseMap _decorationOffsetMapX;
  noise::utils::NoiseMap _decorationOffsetMapZ;
  noise::utils::NoiseMap _decorationRotX;
  noise::utils::NoiseMap _decorationRotY;
  noise::utils::NoiseMap _decorationRotZ;
  noise::utils::NoiseMap _roadMap;

  double _scale;
  int _xOffset;
  int _yOffset;
};