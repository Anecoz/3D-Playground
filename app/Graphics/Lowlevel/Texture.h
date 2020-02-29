#pragma once

#include <GL/glew.h>
#include <string>

class Texture
{
public:
	Texture(unsigned width, unsigned height, GLuint id, GLint type);
	Texture(const std::string& path);

	~Texture();

	void bind();
	void unbind();

	unsigned getWidth() { return _width; }
	unsigned getHeight() { return _height; }
	GLuint getId() { return _texId; }

private:
	unsigned _width, _height;
	GLuint _texId;
	GLint _type = GL_TEXTURE_2D;
};

