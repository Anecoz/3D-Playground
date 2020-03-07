#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

#include <string>
#include <vector>

class Shader
{
public:
	Shader();

  Shader(const Shader&) = delete;
  Shader(Shader&&);

  Shader& operator=(const Shader&) = delete;
  Shader& operator=(Shader&&);

	Shader(const std::string& vertPath, const std::string& fragPath);
  Shader(const std::string& vertPath, const std::string& geomPath, const std::string& fragPath);
  Shader(const std::string& vertPath, const std::string& tescPath, const std::string& tesePath, const std::string& fragPath);
	~Shader();

	static const GLint VERTEX_ATTRIB_LOC = 0;
	static const GLint TEX_ATTRIB_LOC = 1;
  static const GLint NORMAL_ATTRIB_LOC = 2;
  static const GLint COLOR_ATTRIB_LOC = 3;
  static const GLint INSTANCE_MODEL_ATTRIB_LOC_0 = 4;
  static const GLint INSTANCE_MODEL_ATTRIB_LOC_1 = 5;
  static const GLint INSTANCE_MODEL_ATTRIB_LOC_2 = 6;
  static const GLint INSTANCE_MODEL_ATTRIB_LOC_3 = 7;

	GLuint getId() const;
	void bind() const;
	void unbind() const;

	// ---------UPLOADS--------
	void uploadFloat(GLfloat val, const std::string& name) const;
	void uploadInt(GLint val, const std::string& name) const;
	void uploadVec(const glm::vec4& vec, const std::string& name) const;
	void uploadVec(const glm::vec3& vec, const std::string& name) const;
	void uploadVec(const glm::vec2& vec, const std::string& name) const;
	void uploadVecArr(const std::vector<glm::vec2>& vecArr, const std::string& name) const;
	void uploadBool(GLboolean val, const std::string& name) const;
	void uploadTexture(GLint texUnit, const std::string& name) const;
	void uploadMatrix(const glm::mat4& mat, const std::string& name) const;

private:
	GLuint _id;
};

