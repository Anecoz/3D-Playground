#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

#include <string>
#include <vector>

class Shader
{
public:
	Shader();
	Shader(const std::string& vertPath, const std::string& fragPath);
  Shader(const std::string& vertPath, const std::string& tescPath, const std::string& tesePath, const std::string& fragPath);
	~Shader();

	static const GLint VERTEX_ATTRIB_LOC = 0;
	static const GLint TEX_ATTRIB_LOC = 1;
  static const GLint NORMAL_ATTRIB_LOC = 2;
  static const GLint COLOR_ATTRIB_LOC = 3;

	GLuint getId();
	void bind();
	void unbind();

	// ---------UPLOADS--------
	void uploadFloat(GLfloat val, const std::string& name);
	void uploadInt(GLint val, const std::string& name);
	void uploadVec(const glm::vec4& vec, const std::string& name);
	void uploadVec(const glm::vec3& vec, const std::string& name);
	void uploadVec(const glm::vec2& vec, const std::string& name);
	void uploadVecArr(const std::vector<glm::vec2>& vecArr, const std::string& name);
	void uploadBool(GLboolean val, const std::string& name);
	void uploadTexture(GLint texUnit, const std::string& name);
	void uploadMatrix(const glm::mat4& mat, const std::string& name);

private:
	GLuint id;
};

