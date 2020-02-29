#include "NoiseGenerator.h"

#include <stdio.h>

#include <noise/noise.h>

NoiseGenerator::NoiseGenerator(std::size_t mapSize, int xOffset, int yOffset)
  : _mapSize(mapSize + 16)
  , _xOffset(xOffset)
  , _yOffset(yOffset)
{
  using namespace noise;

  module::RidgedMulti mountainTerrain;
  mountainTerrain.SetOctaveCount(3);

  module::Billow baseFlatTerrain;
  baseFlatTerrain.SetFrequency (3.0);
  baseFlatTerrain.SetOctaveCount(6);

  module::ScaleBias flatTerrain;
  flatTerrain.SetSourceModule (0, baseFlatTerrain);
  flatTerrain.SetScale (0.025);
  flatTerrain.SetBias (-0.75);

  module::Perlin terrainType;
  terrainType.SetFrequency (0.5);
  terrainType.SetPersistence (0.25);

  module::Select finalTerrain;
  finalTerrain.SetSourceModule (0, flatTerrain);
  finalTerrain.SetSourceModule (1, mountainTerrain);
  finalTerrain.SetControlModule (terrainType);
  finalTerrain.SetBounds (0.0, 1000.0);
  finalTerrain.SetEdgeFalloff (0.125);

  utils::NoiseMapBuilderPlane heightMapBuilder;
  heightMapBuilder.SetSourceModule (finalTerrain);
  heightMapBuilder.SetDestNoiseMap (_heightMap);
  heightMapBuilder.SetDestSize (_mapSize, _mapSize);
  double scale = 2.0;
  heightMapBuilder.SetBounds (
    (double)_xOffset/(double)_mapSize * scale,
    (double)_xOffset/(double)_mapSize * scale + scale,
    (double)_yOffset/(double)_mapSize * scale,
    (double)_yOffset/(double)_mapSize * scale + scale);
  heightMapBuilder.Build ();
}

double NoiseGenerator::getAt(int x, int z)
{
  if (x < 0 || x > _mapSize || z < 0 || z > _mapSize) {
    return 0.0;
  }

  return 50.0 * _heightMap.GetValue(x, z);
}