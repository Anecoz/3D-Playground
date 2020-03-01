#include "DecorationNoiseGenerator.h"

#include <cstdlib>
#include <noise/noise.h>

DecorationNoiseGenerator::DecorationNoiseGenerator(std::size_t mapSize, int xOffset, int yOffset)
  : _mapSize(mapSize + 16)
  , _xOffset(xOffset)
  , _yOffset(yOffset)
{
  using namespace noise;

  module::Perlin perlinModule;
  perlinModule.SetOctaveCount(1);

  utils::NoiseMapBuilderPlane heightMapBuilder;
  heightMapBuilder.SetSourceModule (perlinModule);
  heightMapBuilder.SetDestNoiseMap (_heightMap);
  heightMapBuilder.SetDestSize (_mapSize, _mapSize);
  heightMapBuilder.SetBounds (6.0, 10.0, 1.0, 5.0);
  heightMapBuilder.Build ();
}

DecorationType DecorationNoiseGenerator::getTypeAt(int x, int y)
{
  if (x < 0 || x > _mapSize || y < 0 || y > _mapSize) {
    return DecorationType::Nothing;
  }

  double val = _heightMap.GetValue(x, y);
  if (val > 0.75) {
    return DecorationType::Rock;
  }
  if (val > -0.25 && val < 0.25) {
    return DecorationType::Grass;
  }
  if (val < -0.75) {
    return DecorationType::Tree;
  }

  return DecorationType::Nothing;
}

bool DecorationNoiseGenerator::kill()
{
  return rand() % 2;
}

double DecorationNoiseGenerator::randomOffset(DecorationType type)
{
  if (type == DecorationType::Tree) {
    return (double)(rand() % 100);
  }
  if (type == DecorationType::Rock) {
    return (double)(rand() % 150);
  }
}

double DecorationNoiseGenerator::randomRotation(int min, int max)
{
  return (double)(rand() % (max - min + 1) + min);
}