#include "ShaderUtils.h"

#include <cstring>
#include <fstream>
#include <iostream>

std::string ShaderUtils::readFile(const char *filePath) {
	std::string content;
	std::ifstream fileStream(filePath, std::ios::in);

	if (!fileStream.is_open()) {
		std::cerr << "Could not read file " << filePath << ". File does not exist." << std::endl;
		return "";
	}

	std::string line = "";
	while (!fileStream.eof()) {
		std::getline(fileStream, line);
		content.append(line + "\n");
	}

	fileStream.close();
	return content;
}

GLuint ShaderUtils::loadShaders(const char *vertex_path, const char *fragment_path)
{
	GLuint vertShader = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);

	// Read shaders
	std::string vertShaderStr = ShaderUtils::readFile(vertex_path);
	std::string fragShaderStr = ShaderUtils::readFile(fragment_path);
	const char *vertShaderSrc = vertShaderStr.c_str();
	const char *fragShaderSrc = fragShaderStr.c_str();

	GLint result = GL_FALSE;
	int logLength;

	// Compile vertex shader
	glShaderSource(vertShader, 1, &vertShaderSrc, NULL);
	glCompileShader(vertShader);

	// Check vertex shader
	glGetShaderiv(vertShader, GL_COMPILE_STATUS, &result);
	glGetShaderiv(vertShader, GL_INFO_LOG_LENGTH, &logLength);
	std::vector<GLchar> vertShaderError((logLength > 1) ? logLength : 1);
	glGetShaderInfoLog(vertShader, logLength, NULL, &vertShaderError[0]);
	if (strlen(&vertShaderError[0]) != 0)
		std::cout << &vertShaderError[0] << std::endl;

	// Compile fragment shader
	glShaderSource(fragShader, 1, &fragShaderSrc, NULL);
	glCompileShader(fragShader);

	// Check fragment shader
	glGetShaderiv(fragShader, GL_COMPILE_STATUS, &result);
	glGetShaderiv(fragShader, GL_INFO_LOG_LENGTH, &logLength);
	std::vector<GLchar> fragShaderError((logLength > 1) ? logLength : 1);
	glGetShaderInfoLog(fragShader, logLength, NULL, &fragShaderError[0]);
	if (strlen(&fragShaderError[0]) != 0)
		std::cout << &fragShaderError[0] << std::endl;

	GLuint program = glCreateProgram();
	glAttachShader(program, vertShader);
	glAttachShader(program, fragShader);
	glLinkProgram(program);

	glGetProgramiv(program, GL_LINK_STATUS, &result);
	glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);
	std::vector<char> programError((logLength > 1) ? logLength : 1);
	glGetProgramInfoLog(program, logLength, NULL, &programError[0]);
	if (strlen(&programError[0]) != 0)
		std::cout << &programError[0] << std::endl;

	glDeleteShader(vertShader);
	glDeleteShader(fragShader);

	return program;
}

GLuint ShaderUtils::loadShaders(const char *vertex_path, const char* geom_path, const char *fragment_path)
{
  GLuint vertShader = glCreateShader(GL_VERTEX_SHADER);
  GLuint geomShader = glCreateShader(GL_GEOMETRY_SHADER);
	GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);

	// Read shaders
	std::string vertShaderStr = ShaderUtils::readFile(vertex_path);
  std::string geomShaderStr = ShaderUtils::readFile(geom_path);
	std::string fragShaderStr = ShaderUtils::readFile(fragment_path);
	const char *vertShaderSrc = vertShaderStr.c_str();
  const char *geomShaderSrc = geomShaderStr.c_str();
	const char *fragShaderSrc = fragShaderStr.c_str();

	GLint result = GL_FALSE;
	int logLength;

	// Compile vertex shader
	glShaderSource(vertShader, 1, &vertShaderSrc, NULL);
	glCompileShader(vertShader);

	// Check vertex shader
	glGetShaderiv(vertShader, GL_COMPILE_STATUS, &result);
	glGetShaderiv(vertShader, GL_INFO_LOG_LENGTH, &logLength);
	std::vector<GLchar> vertShaderError((logLength > 1) ? logLength : 1);
	glGetShaderInfoLog(vertShader, logLength, NULL, &vertShaderError[0]);
	if (strlen(&vertShaderError[0]) != 0)
		std::cout << &vertShaderError[0] << std::endl;

  // Compile geometry shader
	glShaderSource(geomShader, 1, &geomShaderSrc, NULL);
	glCompileShader(geomShader);

	// Check geometry shader
	glGetShaderiv(geomShader, GL_COMPILE_STATUS, &result);
	glGetShaderiv(geomShader, GL_INFO_LOG_LENGTH, &logLength);
	std::vector<GLchar> geomShaderError((logLength > 1) ? logLength : 1);
	glGetShaderInfoLog(geomShader, logLength, NULL, &geomShaderError[0]);
	if (strlen(&geomShaderError[0]) != 0)
		std::cout << &geomShaderError[0] << std::endl;

	// Compile fragment shader
	glShaderSource(fragShader, 1, &fragShaderSrc, NULL);
	glCompileShader(fragShader);

	// Check fragment shader
	glGetShaderiv(fragShader, GL_COMPILE_STATUS, &result);
	glGetShaderiv(fragShader, GL_INFO_LOG_LENGTH, &logLength);
	std::vector<GLchar> fragShaderError((logLength > 1) ? logLength : 1);
	glGetShaderInfoLog(fragShader, logLength, NULL, &fragShaderError[0]);
	if (strlen(&fragShaderError[0]) != 0)
		std::cout << &fragShaderError[0] << std::endl;

	GLuint program = glCreateProgram();
	glAttachShader(program, vertShader);
  glAttachShader(program, geomShader);
	glAttachShader(program, fragShader);
	glLinkProgram(program);

	glGetProgramiv(program, GL_LINK_STATUS, &result);
	glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);
	std::vector<char> programError((logLength > 1) ? logLength : 1);
	glGetProgramInfoLog(program, logLength, NULL, &programError[0]);
	if (strlen(&programError[0]) != 0)
		std::cout << &programError[0] << std::endl;

	glDeleteShader(vertShader);
  glDeleteShader(geomShader);
	glDeleteShader(fragShader);

	return program;
}

GLuint ShaderUtils::loadShaders(const char* vertexPath, const char* tessCPath, const char* tessEPath, const char* fragmentPath)
{
  GLuint vertShader = glCreateShader(GL_VERTEX_SHADER);
  GLuint tessCShader = glCreateShader(GL_TESS_CONTROL_SHADER);
  GLuint tessEShader = glCreateShader(GL_TESS_EVALUATION_SHADER);
  GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);

  // Read shaders
  std::string vertShaderStr = ShaderUtils::readFile(vertexPath);
  std::string tessCShaderStr = ShaderUtils::readFile(tessCPath);
  std::string tessEShaderStr = ShaderUtils::readFile(tessEPath);
  std::string fragShaderStr = ShaderUtils::readFile(fragmentPath);
  
  const char *vertShaderSrc = vertShaderStr.c_str();
  const char *tessCSrc = tessCShaderStr.c_str();
  const char *tessESrc = tessEShaderStr.c_str();
  const char *fragShaderSrc = fragShaderStr.c_str();

  GLint result = GL_FALSE;
  int logLength;

  // Compile vertex shader
  glShaderSource(vertShader, 1, &vertShaderSrc, NULL);
  glCompileShader(vertShader);

  // Check vertex shader
  glGetShaderiv(vertShader, GL_COMPILE_STATUS, &result);
  glGetShaderiv(vertShader, GL_INFO_LOG_LENGTH, &logLength);
  std::vector<GLchar> vertShaderError((logLength > 1) ? logLength : 1);
  glGetShaderInfoLog(vertShader, logLength, NULL, &vertShaderError[0]);
  if (strlen(&vertShaderError[0]) != 0)
    std::cout << &vertShaderError[0] << std::endl;

  // Compile tess control shader
  glShaderSource(tessCShader, 1, &tessCSrc, NULL);
  glCompileShader(tessCShader);

  // Check tess control shader
  glGetShaderiv(tessCShader, GL_COMPILE_STATUS, &result);
  glGetShaderiv(tessCShader, GL_INFO_LOG_LENGTH, &logLength);
  std::vector<GLchar> controlShaderError((logLength > 1) ? logLength : 1);
  glGetShaderInfoLog(tessCShader, logLength, NULL, &controlShaderError[0]);
  if (strlen(&controlShaderError[0]) != 0)
    std::cout << &controlShaderError[0] << std::endl;

  // Compile tess eval shader
  glShaderSource(tessEShader, 1, &tessESrc, NULL);
  glCompileShader(tessEShader);

  // Check tess eval shader
  glGetShaderiv(tessEShader, GL_COMPILE_STATUS, &result);
  glGetShaderiv(tessEShader, GL_INFO_LOG_LENGTH, &logLength);
  std::vector<GLchar> evalShaderError((logLength > 1) ? logLength : 1);
  glGetShaderInfoLog(tessEShader, logLength, NULL, &evalShaderError[0]);
  if (strlen(&evalShaderError[0]) != 0)
    std::cout << &evalShaderError[0] << std::endl;

  // Compile fragment shader
  glShaderSource(fragShader, 1, &fragShaderSrc, NULL);
  glCompileShader(fragShader);

  // Check fragment shader
  glGetShaderiv(fragShader, GL_COMPILE_STATUS, &result);
  glGetShaderiv(fragShader, GL_INFO_LOG_LENGTH, &logLength);
  std::vector<GLchar> fragShaderError((logLength > 1) ? logLength : 1);
  glGetShaderInfoLog(fragShader, logLength, NULL, &fragShaderError[0]);
  if (strlen(&fragShaderError[0]) != 0)
    std::cout << &fragShaderError[0] << std::endl;

  GLuint program = glCreateProgram();
  glAttachShader(program, vertShader);
  glAttachShader(program, tessCShader);
  glAttachShader(program, tessEShader);
  glAttachShader(program, fragShader);
  glLinkProgram(program);

  glGetProgramiv(program, GL_LINK_STATUS, &result);
  glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);
  std::vector<char> programError((logLength > 1) ? logLength : 1);
  glGetProgramInfoLog(program, logLength, NULL, &programError[0]);
  if (strlen(&programError[0]) != 0)
    std::cout << &programError[0] << std::endl;

  glDeleteShader(vertShader);
  glDeleteShader(tessCShader);
  glDeleteShader(tessEShader);
  glDeleteShader(fragShader);

  return program;
}
