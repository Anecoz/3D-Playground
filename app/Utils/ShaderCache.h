#pragma once

#include "../Graphics/Lowlevel/Shader.h"

#include <map>

enum class ShaderType
{
  Water,
  InstancedModel,
  Grid,
  ShadowGrid,
  ShadowInstancedModel
};

class ShaderCache
{
public:
  ShaderCache() = delete;

  static void addShader(Shader&&, ShaderType type);
  static const Shader& getShader(ShaderType type);

private:
  static std::map<ShaderType, Shader> _cachedShaders;
};