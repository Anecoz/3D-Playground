#include "ShaderCache.h"

#include <cstdio>

std::map<ShaderType, Shader> ShaderCache::_cachedShaders;

static Shader g_EmptyShader = Shader();

void ShaderCache::addShader(Shader&& shader, ShaderType type)
{
  if (_cachedShaders.find(type) == _cachedShaders.end()) {
    _cachedShaders[type] = std::move(shader);
  }
}

const Shader& ShaderCache::getShader(ShaderType type)
{
  if (_cachedShaders.find(type) != _cachedShaders.end()) {
    return _cachedShaders[type];
  }

  printf("Shader cache cannot find shader of type %d, returning invalid shader!\n", static_cast<int>(type));
  return g_EmptyShader;
}