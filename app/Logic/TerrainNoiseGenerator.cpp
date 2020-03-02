#include "TerrainNoiseGenerator.h"

#include <stdio.h>

#include <noise/noise.h>

NoiseGenerator::NoiseGenerator(std::size_t mapSize, int xOffset, int yOffset)
  : _mapSize(mapSize + 16)
  , _xOffset(xOffset)
  , _yOffset(yOffset)
{
  using namespace noise;

  /*module::RidgedMulti mountainTerrain;
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
  finalTerrain.SetEdgeFalloff (0.125);*/

  module::RidgedMulti baseMountainTerrain;
  module::ScaleBias mountainTerrain;
  mountainTerrain.SetSourceModule(0, baseMountainTerrain);
  mountainTerrain.SetBias(0.5);

  module::Billow baseFlatTerrain;
  baseFlatTerrain.SetFrequency (2.0);

  module::ScaleBias flatTerrain;
  flatTerrain.SetSourceModule (0, baseFlatTerrain);
  flatTerrain.SetScale (0.025);
  flatTerrain.SetBias (-0.25);

  module::RidgedMulti river;
  river.SetFrequency(0.05);
  river.SetOctaveCount(1);
  river.SetLacunarity(3.0);

  module::ScaleBias preInvertScaleRiver;
  preInvertScaleRiver.SetSourceModule(0, river);
  preInvertScaleRiver.SetScale(4.0);

  module::Const val;
  val.SetConstValue(3.0);

  module::Power riverPower;
  riverPower.SetSourceModule(0, preInvertScaleRiver);
  riverPower.SetSourceModule(1, val);

  module::Invert riverInvert;
  riverInvert.SetSourceModule(0, riverPower);
  
  module::ScaleBias riverBias;
  riverBias.SetSourceModule(0, riverInvert);
  riverBias.SetScale(1.5);

  module::Select riverMask;
  riverMask.SetSourceModule (0, flatTerrain);
  riverMask.SetSourceModule (1, riverBias);
  riverMask.SetControlModule (riverBias);
  riverMask.SetBounds (-10.0, -0.5);
  riverMask.SetEdgeFalloff (0.8);

  module::Perlin terrainType;
  terrainType.SetFrequency (0.5);
  terrainType.SetPersistence (0.25);

  module::Select finalDryTerrain;
  finalDryTerrain.SetSourceModule (0, flatTerrain);
  finalDryTerrain.SetSourceModule (1, mountainTerrain);
  finalDryTerrain.SetControlModule (terrainType);
  finalDryTerrain.SetBounds (0.0, 1000.0);
  finalDryTerrain.SetEdgeFalloff (0.125);

  module::Select finalTerrain;
  finalTerrain.SetSourceModule(0, finalDryTerrain);
  finalTerrain.SetSourceModule(1, riverMask);
  finalTerrain.SetControlModule(riverMask);
  finalTerrain.SetBounds(-10.0, -0.5);
  finalTerrain.SetEdgeFalloff(0.8);

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