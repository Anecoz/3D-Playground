#include "ObjModelCache.h"

#include <stdio.h>

std::map<CachedModelType, OBJData> ObjModelCache::_cachedData;

void ObjModelCache::cacheModel(const std::string& objPath, CachedModelType type)
{
  std::string directory = objPath;
  const size_t lastSlashIdx = objPath.rfind('/');
  if (std::string::npos != lastSlashIdx) {
    directory = objPath.substr(0, lastSlashIdx);
  }

  OBJData data;
  if (OBJLoader::loadFromFile(objPath, directory, data)) {
    printf("Cached model, %zu verts and %zu indices\n", data._vertices.size(), data._indices.size());
    _cachedData[type] = std::move(data);
  }
}

bool ObjModelCache::getCachedModel(CachedModelType type, OBJData* dataOut)
{
  if (_cachedData.find(type) != _cachedData.end()) {
    *dataOut = _cachedData[type];
    return true;
  }

  printf("Model type %d is not cached!\n", static_cast<int>(type));
  return false;
}