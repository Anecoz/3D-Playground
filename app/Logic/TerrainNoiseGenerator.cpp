#include "TerrainNoiseGenerator.h"

#include <stdio.h>

#include <noise/noise.h>

using namespace noise;

NoiseGenerator::NoiseGenerator(std::size_t mapSize, int xOffset, int yOffset)
  : _mapSize(mapSize + _mapBufferSize)
  , _scale(((double)_mapSize * 3.5)/512.0)
  , _xOffset(xOffset)
  , _yOffset(yOffset)
{
  buildHeightMap();
  buildDecorationMap();
  buildDecorationScaleMap();
  buildDecorationOffsetMaps();
  buildDecorationRotMaps();
  buildRoadMaps();
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

  module::Perlin baseFlatTerrain;
  baseFlatTerrain.SetFrequency (1.0);
  baseFlatTerrain.SetOctaveCount(4);

  module::ScaleBias flatTerrain;
  flatTerrain.SetSourceModule (0, baseFlatTerrain);
  flatTerrain.SetScale (0.15);
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

  module::RidgedMulti roads;
  roads.SetFrequency(0.02);
  roads.SetOctaveCount(1);
  roads.SetLacunarity(0.1);
  roads.SetSeed(12398);

  module::ScaleBias preInvertScaleRoad;
  preInvertScaleRoad.SetSourceModule(0, roads);
  preInvertScaleRoad.SetScale(4.0);

  module::Invert roadsInvert;
  roadsInvert.SetSourceModule(0, preInvertScaleRoad);
  
  module::Clamp roadClamp;
  roadClamp.SetSourceModule(0, roadsInvert);
  roadClamp.SetBounds(-1.0, -0.9);

  module::Select finalFinalTerrain;
  finalFinalTerrain.SetSourceModule(0, finalTerrain);
  finalFinalTerrain.SetSourceModule(1, flatTerrain);
  finalFinalTerrain.SetControlModule(roadClamp);
  finalFinalTerrain.SetBounds(-2.0, -0.9);
  finalFinalTerrain.SetEdgeFalloff(0.05);

  utils::NoiseMapBuilderPlane heightMapBuilder;
  heightMapBuilder.SetSourceModule (finalFinalTerrain);
  heightMapBuilder.SetDestNoiseMap (_heightMap);
  heightMapBuilder.SetDestSize ((int)_mapSize, (int)_mapSize);

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
  heightMapBuilder.SetDestSize ((int)_mapSize, (int)_mapSize);

  heightMapBuilder.SetBounds (
    (double)_xOffset/(double)_mapSize * _scale,
    (double)_xOffset/(double)_mapSize * _scale + _scale,
    (double)_yOffset/(double)_mapSize * _scale,
    (double)_yOffset/(double)_mapSize * _scale + _scale);
  heightMapBuilder.Build ();
}

void NoiseGenerator::buildDecorationScaleMap()
{
  module::Perlin placementHelper;
  placementHelper.SetFrequency(4.0);
  placementHelper.SetOctaveCount(3);

  module::ScaleBias biaser;
  biaser.SetSourceModule(0, placementHelper);
  biaser.SetBias(1.0);

  module::Clamp clamper;
  clamper.SetSourceModule(0, biaser);
  clamper.SetBounds(0.7, 2.0);

  utils::NoiseMapBuilderPlane heightMapBuilder;
  heightMapBuilder.SetSourceModule (clamper);
  heightMapBuilder.SetDestNoiseMap (_decorationScaleMap);
  heightMapBuilder.SetDestSize ((int)_mapSize, (int)_mapSize);

  heightMapBuilder.SetBounds (
    (double)_xOffset/(double)_mapSize * _scale,
    (double)_xOffset/(double)_mapSize * _scale + _scale,
    (double)_yOffset/(double)_mapSize * _scale,
    (double)_yOffset/(double)_mapSize * _scale + _scale);
  heightMapBuilder.Build ();
}

void NoiseGenerator::buildDecorationOffsetMaps()
{
  // X
  module::Perlin placementHelperX;
  placementHelperX.SetFrequency(4.0);
  placementHelperX.SetOctaveCount(3);

  module::ScaleBias scaleBiasX;
  scaleBiasX.SetSourceModule(0, placementHelperX);
  scaleBiasX.SetScale(100.0);

  utils::NoiseMapBuilderPlane heightMapBuilderX;
  heightMapBuilderX.SetSourceModule (scaleBiasX);
  heightMapBuilderX.SetDestNoiseMap (_decorationOffsetMapX);
  heightMapBuilderX.SetDestSize ((int)_mapSize, (int)_mapSize);

  heightMapBuilderX.SetBounds (
    (double)_xOffset/(double)_mapSize * _scale,
    (double)_xOffset/(double)_mapSize * _scale + _scale,
    (double)_yOffset/(double)_mapSize * _scale,
    (double)_yOffset/(double)_mapSize * _scale + _scale);
  heightMapBuilderX.Build ();

  // Z
  module::Perlin placementHelperZ;
  placementHelperZ.SetFrequency(2.0);
  placementHelperZ.SetOctaveCount(3);

  module::ScaleBias scaleBiasZ;
  scaleBiasZ.SetSourceModule(0, placementHelperZ);
  scaleBiasZ.SetScale(100.0);

  utils::NoiseMapBuilderPlane heightMapBuilderZ;
  heightMapBuilderZ.SetSourceModule (scaleBiasZ);
  heightMapBuilderZ.SetDestNoiseMap (_decorationOffsetMapZ);
  heightMapBuilderZ.SetDestSize ((int)_mapSize, (int)_mapSize);

  heightMapBuilderZ.SetBounds (
    (double)_xOffset/(double)_mapSize * _scale,
    (double)_xOffset/(double)_mapSize * _scale + _scale,
    (double)_yOffset/(double)_mapSize * _scale,
    (double)_yOffset/(double)_mapSize * _scale + _scale);
  heightMapBuilderZ.Build ();
}

void NoiseGenerator::buildDecorationRotMaps()
{
  // X
  module::Perlin placementHelperX;
  placementHelperX.SetFrequency(2.0);
  placementHelperX.SetOctaveCount(3);

  module::ScaleBias scaleBiasX;
  scaleBiasX.SetSourceModule(0, placementHelperX);
  scaleBiasX.SetScale(10.0);

  utils::NoiseMapBuilderPlane heightMapBuilderX;
  heightMapBuilderX.SetSourceModule (scaleBiasX);
  heightMapBuilderX.SetDestNoiseMap (_decorationRotX);
  heightMapBuilderX.SetDestSize ((int)_mapSize, (int)_mapSize);

  heightMapBuilderX.SetBounds (
    (double)_xOffset/(double)_mapSize * _scale,
    (double)_xOffset/(double)_mapSize * _scale + _scale,
    (double)_yOffset/(double)_mapSize * _scale,
    (double)_yOffset/(double)_mapSize * _scale + _scale);
  heightMapBuilderX.Build ();

  // Y
  module::Perlin placementHelperY;
  placementHelperY.SetFrequency(2.0);
  placementHelperY.SetOctaveCount(3);

  module::ScaleBias scaleBiasY;
  scaleBiasY.SetSourceModule(0, placementHelperY);
  scaleBiasY.SetScale(180.0);
  scaleBiasY.SetBias(180.0);

  utils::NoiseMapBuilderPlane heightMapBuilderY;
  heightMapBuilderY.SetSourceModule (scaleBiasY);
  heightMapBuilderY.SetDestNoiseMap (_decorationRotY);
  heightMapBuilderY.SetDestSize ((int)_mapSize, (int)_mapSize);

  heightMapBuilderY.SetBounds (
    (double)_xOffset/(double)_mapSize * _scale,
    (double)_xOffset/(double)_mapSize * _scale + _scale,
    (double)_yOffset/(double)_mapSize * _scale,
    (double)_yOffset/(double)_mapSize * _scale + _scale);
  heightMapBuilderY.Build ();

  // Z
  module::Perlin placementHelperZ;
  placementHelperZ.SetFrequency(2.0);
  placementHelperZ.SetOctaveCount(3);

  module::ScaleBias scaleBiasZ;
  scaleBiasZ.SetSourceModule(0, placementHelperZ);
  scaleBiasZ.SetScale(10.0);

  utils::NoiseMapBuilderPlane heightMapBuilderZ;
  heightMapBuilderZ.SetSourceModule (scaleBiasZ);
  heightMapBuilderZ.SetDestNoiseMap (_decorationRotZ);
  heightMapBuilderZ.SetDestSize ((int)_mapSize, (int)_mapSize);

  heightMapBuilderZ.SetBounds (
    (double)_xOffset/(double)_mapSize * _scale,
    (double)_xOffset/(double)_mapSize * _scale + _scale,
    (double)_yOffset/(double)_mapSize * _scale,
    (double)_yOffset/(double)_mapSize * _scale + _scale);
  heightMapBuilderZ.Build ();
}

void NoiseGenerator::buildRoadMaps()
{
  module::RidgedMulti roads;
  roads.SetFrequency(0.02);
  roads.SetOctaveCount(1);
  roads.SetLacunarity(0.1);
  roads.SetSeed(12398);

  module::ScaleBias preInvertScaleRoad;
  preInvertScaleRoad.SetSourceModule(0, roads);
  preInvertScaleRoad.SetScale(4.0);

  module::Invert roadsInvert;
  roadsInvert.SetSourceModule(0, preInvertScaleRoad);
  
  module::Clamp clamp;
  clamp.SetSourceModule(0, roadsInvert);
  clamp.SetBounds(-1.0, -0.9);

  utils::NoiseMapBuilderPlane builder;
  builder.SetSourceModule (clamp);
  builder.SetDestNoiseMap (_roadMap);
  builder.SetDestSize ((int)_mapSize, (int)_mapSize);

  builder.SetBounds (
    (double)_xOffset/(double)_mapSize * _scale,
    (double)_xOffset/(double)_mapSize * _scale + _scale,
    (double)_yOffset/(double)_mapSize * _scale,
    (double)_yOffset/(double)_mapSize * _scale + _scale);
  builder.Build ();
}

double NoiseGenerator::getHeightAt(int x, int z)
{
  x += (int)_mapBufferSize/2;
  z += (int)_mapBufferSize/2;
  if (x <= 0 || x >= _mapSize || z <= 0 || z >= _mapSize) {
    return 10000000.0;
  }

  return 50.0 * _heightMap.GetValue(x, z);
}

DecorationData NoiseGenerator::getDecorationDataAt(int x, int z)
{
  DecorationData data;
  data._type = DecorationType::Nothing;

  x += (int)_mapBufferSize/2;
  z +=(int) _mapBufferSize/2;
  if (x < 0 || x > _mapSize || z < 0 || z > _mapSize) {
    return data;
  }

  // For sparsity
  if (x % 6 != 0 || z % 6 != 0) {
    return data;
  }  

  // Sample decorationMap, use value to determine type
  double value = _decorationMap.GetValue(x, z);
  double scale = _decorationScaleMap.GetValue(x, z);
  double offsetX = _decorationOffsetMapX.GetValue(x, z);
  double offsetZ = _decorationOffsetMapZ.GetValue(x, z);
  double rotX = _decorationRotX.GetValue(x, z);
  double rotY = _decorationRotY.GetValue(x, z);
  double rotZ = _decorationRotZ.GetValue(x, z);

  double height = getHeightAt(x, z);
  double roadVal = _roadMap.GetValue(x, z);

  if (roadVal < -0.99) {
    data._type = DecorationType::Fence;
    data._scale = 10.0;
    data._offsetX = 0.0;
    data._offsetZ = 0.0;
    data._xRotDeg = 0.0;
    data._yRotDeg = 0.0;
    data._zRotDeg = 0.0;
  }
  else {
    // The output of the decoration map (for now) is binary, -1 or 1
    if (value < 0.5) {
      if (height > -13.5 && height < -11.5) {
        data._type = DecorationType::Tree;
      }
      else if (height > -11.5 && height < -10.0) {
        scale *= 1.2;
        rotX *= 0.5;
        rotY *= 0.5;
        rotZ *= 0.5;
        data._type = DecorationType::Tree2;
      }
      else if (height > -10.0 && height < -8.0) {
        data._type = DecorationType::Tree3;
      }
      else if (height > -8.0 && height < -5.0) {
        scale *= 0.5;
        data._type = DecorationType::SmallRock;
      }
      else if (height > 15.0) {
        data._type = DecorationType::Rock;
      }
      else {
        scale *= 0.5;
        data._type = DecorationType::Grass;
      }
    }
    else {
      // Grass
      data._type = DecorationType::Grass;
      scale *= 0.5;
    }
    data._scale = scale;
    data._offsetX = offsetX;
    data._offsetZ = offsetZ;
    data._xRotDeg = rotX;
    data._yRotDeg = rotY;
    data._zRotDeg = rotZ;
  }  

  return data;
}