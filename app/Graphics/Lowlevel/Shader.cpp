#include "Shader.h"
#include "../../Utils/ShaderUtils.h"
#include <glm/gtc/type_ptr.hpp>

Shader::Shader()
  : _id(0)
{}

Shader::Shader(Shader&& other)
{
  _id = other._id;
  other._id = 0;
}

Shader& Shader::operator=(Shader&& other)
{
  if (&other != this) {
    _id = other._id;
    other._id = 0;
  }
  return *this;
}

Shader::Shader(const std::string& vertPath, const std::string& fragPath)
  : _id(ShaderUtils::loadShaders(vertPath.c_str(), fragPath.c_str()))
{}

Shader::Shader(const std::string& vertPath, const std::string& geomPath, const std::string& fragPath)
  : _id(ShaderUtils::loadShaders(vertPath.c_str(), geomPath.c_str(), fragPath.c_str()))
{}

Shader::Shader(const std::string& vertPath, const std::string& tescPath, const std::string& tesePath, const std::string& fragPath)
  : _id(ShaderUtils::loadShaders(vertPath.c_str(), tescPath.c_str(), tesePath.c_str(), fragPath.c_str()))
{}

Shader::~Shader()
{
  if (_id != 0) {
    glDeleteProgram(_id);
  }
}

GLuint Shader::getId() const
{
	return _id;
}

void Shader::bind() const
{
	glUseProgram(_id);
}

void Shader::unbind() const
{
	glUseProgram(0);
}

void Shader::uploadBool(GLboolean val, const std::string& name) const
{
  GLint loc = getUniformLocation(name);
	glUniform1i(loc, val);
}

void Shader::uploadFloat(GLfloat val, const std::string& name) const
{
	GLint loc = getUniformLocation(name);
	glUniform1f(loc, val);
}

void Shader::uploadInt(GLint val, const std::string& name) const
{
	GLint loc = getUniformLocation(name);
	glUniform1i(loc, val);
}

void Shader::uploadMatrix(const glm::mat4& mat, const std::string& name) const
{
	GLint loc = getUniformLocation(name);
	glUniformMatrix4fv(loc, 1, GL_FALSE, &mat[0][0]);
}

void Shader::uploadTexture(GLint texUnit, const std::string& name) const
{
	GLint loc = getUniformLocation(name);
	glUniform1i(loc, texUnit);
}

void Shader::uploadVec(const glm::vec2& vec, const std::string& name) const
{
	GLint loc = getUniformLocation(name);
	glUniform2fv(loc, 1, value_ptr(vec));
}

void Shader::uploadVec(const glm::vec3& vec, const std::string& name) const
{
	GLint loc = getUniformLocation(name);
	glUniform3fv(loc, 1, value_ptr(vec));
}

void Shader::uploadVec(const glm::vec4& vec, const std::string& name) const
{
	GLint loc = getUniformLocation(name);
	glUniform4fv(loc, 1, value_ptr(vec));
}

void Shader::uploadVecArr(const std::vector<glm::vec2>& vecArr, const std::string& name) const
{
	GLint loc = getUniformLocation(name);
	GLint size = (GLint)vecArr.size();
	glUniform3fv(loc, size, &vecArr[0].x);
}

GLint Shader::getUniformLocation(const std::string& name) const
{
  auto it = _uniformLocationCache.find(name);
  if (it != _uniformLocationCache.end()) {
    return it->second;
  }

  GLint loc = glGetUniformLocation(_id, name.c_str());
  _uniformLocationCache[name] = loc;
  return loc;
}