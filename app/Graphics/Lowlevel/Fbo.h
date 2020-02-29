#pragma once

#include <GL/glew.h>

class Fbo
{
public:
	Fbo(int width, int height);
	~Fbo();

	void bind();
	void unbind();

	GLuint getColourTextureId() { return _colourTextureId; }

private:
	int _width;
	int _height;
	GLuint _fboId, _colourTextureId;

	void initialiseFramebuffer();
	void createFramebuffer();
	void createTextureAttachment();
};

