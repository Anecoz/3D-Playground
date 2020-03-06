#pragma once

#include <GL/glew.h>

class Fbo
{
public:
	Fbo(int width, int height, bool color, bool depth);
	~Fbo();

	void bind();
	void unbind();

	GLuint getColourTextureId() { return _colourTextureId; }
  GLuint getDepthTextureId() { return _depthTextureId; }

private:
	int _width;
	int _height;
  bool _color;
  bool _depth;
	GLuint _fboId, _colourTextureId, _depthTextureId;

	void initialiseFramebuffer();
	void createFramebuffer();
	void createTextureAttachment();
  void createDepthBufferAttachment();
};

