#include "TerrainNoiseGenerator.h"

#include <stdio.h>

#include <noise/noise.h>

using namespace noise;

NoiseGenerator::NoiseGenerator(std::size_t mapSize, int xOffset, int yOffset)
  : _mapSize(mapSize + 16)
  , _scale(3.5)
  , _xOffset(xOffset)
  , _yOffset(yOffset)
{
  buildHeightMap();
  buildDecorationMap();
}

void NoiseGenerator::buildHeightMap()
{
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

  module::Perlin baseMountainTerrain;
  baseMountainTerrain.SetOctaveCount(6);
  baseMountainTerrain.SetFrequency(0.25);

  module::ScaleBias mountainTerrain;
  mountainTerrain.SetSourceModule(0, baseMountainTerrain);
  mountainTerrain.SetScale(1.0);
  mountainTerrain.SetBias(2.0);

  module::Billow baseFlatTerrain;
  baseFlatTerrain.SetFrequency (2.0);
  baseFlatTerrain.SetOctaveCount(2);

  module::ScaleBias flatTerrain;
  flatTerrain.SetSourceModule (0, baseFlatTerrain);
  flatTerrain.SetScale (0.070);
  flatTerrain.SetBias (-0.25);

  module::RidgedMulti river;
  river.SetFrequency(0.05);
  river.SetOctaveCount(1);
  river.SetLacunarity(100.0);

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
  riverBias.SetScale(2.0);

  module::Select riverMask;
  riverMask.SetSourceModule (0, flatTerrain);
  riverMask.SetSourceModule (1, riverBias);
  riverMask.SetControlModule (riverBias);
  riverMask.SetBounds (-10.0, -0.5);
  riverMask.SetEdgeFalloff (0.8);

  module::Perlin terrainType;
  terrainType.SetFrequency (0.15);
  terrainType.SetPersistence (0.5);

  module::Select finalDryTerrain;
  finalDryTerrain.SetSourceModule (0, flatTerrain);
  finalDryTerrain.SetSourceModule (1, mountainTerrain);
  finalDryTerrain.SetControlModule (terrainType);
  finalDryTerrain.SetBounds (0.2, 1000.0);
  finalDryTerrain.SetEdgeFalloff (0.25);

  module::Select finalTerrain;
  finalTerrain.SetSourceModule(0, finalDryTerrain);
  finalTerrain.SetSourceModule(1, riverMask);
  finalTerrain.SetControlModule(riverMask);
  finalTerrain.SetBounds(-10.0, -0.5);
  finalTerrain.SetEdgeFalloff(0.125);

  utils::NoiseMapBuilderPlane heightMapBuilder;
  heightMapBuilder.SetSourceModule (finalTerrain);
  heightMapBuilder.SetDestNoiseMap (_heightMap);
  heightMapBuilder.SetDestSize (_mapSize, _mapSize);

  heightMapBuilder.SetBounds (
    (double)_xOffset/(double)_mapSize * _scale,
    (double)_xOffset/(double)_mapSize * _scale + _scale,
    (double)_yOffset/(double)_mapSize * _scale,
    (double)_yOffset/(double)_mapSize * _scale + _scale);
  heightMapBuilder.Build ();
}

void NoiseGenerator::buildDecorationMap()
{
  module::Perlin placementHelper;
  placementHelper.SetFrequency(0.3);
  placementHelper.SetOctaveCount(2);

  module::Const val1;
  val1.SetConstValue(1.0);
  module::Const val2;
  val2.SetConstValue(-1.0);

  module::Select sel;
  sel.SetSourceModule(0, val1);
  sel.SetSourceModule(1, placementHelper);
  sel.SetControlModule(placementHelper);
  sel.SetBounds(-10.0, -0.3);
  sel.SetEdgeFalloff(0.00);

  module::Select binary;
  binary.SetSourceModule(0, val1);
  binary.SetSourceModule(1, val2);
  binary.SetControlModule(sel);
  binary.SetBounds(-10, 0.7);

  utils::NoiseMapBuilderPlane heightMapBuilder;
  heightMapBuilder.SetSourceModule (binary);
  heightMapBuilder.SetDestNoiseMap (_decorationMap);
  heightMapBuilder.SetDestSize (_mapSize, _mapSize);

  heightMapBuilder.SetBounds (
    (double)_xOffset/(double)_mapSize * _scale,
    (double)_xOffset/(double)_mapSize * _scale + _scale,
    (double)_yOffset/(double)_mapSize * _scale,
    (double)_yOffset/(double)_mapSize * _scale + _scale);
  heightMapBuilder.Build ();
}

double NoiseGenerator::getHeightAt(int x, int z)
{
  if (x < 0 || x > _mapSize || z < 0 || z > _mapSize) {
    return 0.0;
  }

  return 50.0 * _heightMap.GetValue(x, z);
}

DecorationData NoiseGenerator::getDecorationDataAt(int x, int z)
{
  DecorationData data;
  if (x < 0 || x > _mapSize || z < 0 || z > _mapSize) {
    data._type = DecorationType::Nothing;
    return data;
  }

  // TEST
  if (x % 8 != 0 || z % 8 != 0) {
    data._type = DecorationType::Nothing;
    return data;
  }

  // Sample decorationMap, use value to determine type
  double value = _decorationMap.GetValue(x, z);
  double height = getHeightAt(x, z);

  /*if (height > -10.0 || height < -15.0) {
    data._type = DecorationType::Nothing;
    return data;
  }*/

  if (value >= -10.0 && value <= 0.8) {
    data._type = DecorationType::Tree;
    data._scale = 1.0;
    data._xRotDeg = 0.0;
    data._yRotDeg = 0.0;
    data._zRotDeg = 0.0;
  }

  return data;
}