#include "Shader.h"
#include "../../Utils/ShaderUtils.h"
#include <glm/gtc/type_ptr.hpp>

Shader::Shader() {
	id = -1;
}

Shader::Shader(const std::string& vertPath, const std::string& fragPath) {
	id = ShaderUtils::loadShaders(vertPath.c_str(), fragPath.c_str());
}

Shader::Shader(const std::string& vertPath, const std::string& geomPath, const std::string& fragPath) {
	id = ShaderUtils::loadShaders(vertPath.c_str(), geomPath.c_str(), fragPath.c_str());
}

Shader::Shader(const std::string& vertPath, const std::string& tescPath, const std::string& tesePath, const std::string& fragPath) {
  id = ShaderUtils::loadShaders(vertPath.c_str(), tescPath.c_str(), tesePath.c_str(), fragPath.c_str());
}

Shader::~Shader() {
	glDeleteProgram(id);
}

GLuint Shader::getId() {
	return id;
}

void Shader::bind() {
	glUseProgram(id);
}

void Shader::unbind() {
	glUseProgram(0);
}

void Shader::uploadBool(GLboolean val, const std::string& name) {
	GLint loc = glGetUniformLocation(id, name.c_str());
	glUniform1i(loc, val);
}

void Shader::uploadFloat(GLfloat val, const std::string& name) {
	GLint loc = glGetUniformLocation(id, name.c_str());
	glUniform1f(loc, val);
}

void Shader::uploadInt(GLint val, const std::string& name) {
	GLint loc = glGetUniformLocation(id, name.c_str());
	glUniform1i(loc, val);
}

void Shader::uploadMatrix(const glm::mat4& mat, const std::string& name) {
	GLint loc = glGetUniformLocation(id, name.c_str());
	glUniformMatrix4fv(loc, 1, GL_FALSE, &mat[0][0]);
}

void Shader::uploadTexture(GLint texUnit, const std::string& name) {
	GLint loc = glGetUniformLocation(id, name.c_str());
	glUniform1i(loc, texUnit);
}

void Shader::uploadVec(const glm::vec2& vec, const std::string& name) {
	GLint loc = glGetUniformLocation(id, name.c_str());
	glUniform2fv(loc, 1, value_ptr(vec));
}

void Shader::uploadVec(const glm::vec3& vec, const std::string& name) {
	GLint loc = glGetUniformLocation(id, name.c_str());
	glUniform3fv(loc, 1, value_ptr(vec));
}

void Shader::uploadVec(const glm::vec4& vec, const std::string& name) {
	GLint loc = glGetUniformLocation(id, name.c_str());
	glUniform4fv(loc, 1, value_ptr(vec));
}

void Shader::uploadVecArr(const std::vector<glm::vec2>& vecArr, const std::string& name) {
	GLint loc = glGetUniformLocation(id, name.c_str());
	GLint size = vecArr.size();
	glUniform3fv(loc, size, &vecArr[0].x);
}