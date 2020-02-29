#pragma once
#include <GL/glew.h>

#include <string>
#include <vector>

class ShaderUtils {

public:
	static std::string readFile(const char* filePath);
	static GLuint loadShaders(const char *vertex_path, const char *fragment_path);
  static GLuint loadShaders(const char* vertexPath, const char* tessCPath, const char* tessEPath, const char* fragmentPath);
};