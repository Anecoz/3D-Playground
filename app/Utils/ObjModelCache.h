#pragma once

#include "OBJLoader.h"

#include <string>
#include <map>

enum class CachedModelType
{
  Tree,
  Rock,
  Grass
};

class ObjModelCache
{
public:
  ObjModelCache() = default;
  ~ObjModelCache() = default;
  
  static void cacheModel(const std::string& objPath, CachedModelType type);
  
  static bool getCachedModel(CachedModelType type, OBJData* out);

private:
  static std::map<CachedModelType, OBJData> _cachedData;
};