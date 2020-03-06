#include "Fbo.h"

#include <cstdio>

Fbo::Fbo(int width, int height, bool color, bool depth)
  : _width(width)
  , _height(height)
  , _color(color)
  , _depth(depth)
{
	initialiseFramebuffer();
}

Fbo::~Fbo()
{
	glDeleteFramebuffers(1, &_fboId);
	glDeleteTextures(1, &_colourTextureId);
}

void Fbo::bind()
{
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, _fboId);
	glViewport(0, 0, _width, _height);
}

void Fbo::unbind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, 1920, 1080); // ADD CORRECT RESOLUTION
}

void Fbo::initialiseFramebuffer()
{
	createFramebuffer();
  //if (_color) {
    createTextureAttachment();
  //}
  if (_depth) {
    createDepthBufferAttachment();
  }

  auto ret = glCheckFramebufferStatus(GL_FRAMEBUFFER);
  if (ret != GL_FRAMEBUFFER_COMPLETE) {
    printf("Framebuffer status not complete: %d\n", ret);
  }

	unbind();
}

void Fbo::createFramebuffer()
{
	glGenFramebuffers(1, &_fboId);
	glBindFramebuffer(GL_FRAMEBUFFER, _fboId);
  glDrawBuffer(GL_COLOR_ATTACHMENT0);
}

void Fbo::createTextureAttachment()
{
	glGenTextures(1, &_colourTextureId);
	glBindTexture(GL_TEXTURE_2D, _colourTextureId);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, _width, _height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _colourTextureId, 0);
}

void Fbo::createDepthBufferAttachment()
{
	glGenTextures(1, &_depthTextureId);
	glBindTexture(GL_TEXTURE_2D, _depthTextureId);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, _width, _height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, _depthTextureId, 0);
}