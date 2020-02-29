#include "Fbo.h"

Fbo::Fbo(int width, int height) {
  _width = width;
  _height = height;
	initialiseFramebuffer();
}

Fbo::~Fbo() {
	glDeleteFramebuffers(1, &_fboId);
	glDeleteTextures(1, &_colourTextureId);
}

void Fbo::bind() {
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, _fboId);
	glViewport(0, 0, _width, _height);
}

void Fbo::unbind() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, 1280, 720); // ADD CORRECT RESOLUTION
}

void Fbo::initialiseFramebuffer() {
	createFramebuffer();
	createTextureAttachment();
	unbind();
}

void Fbo::createFramebuffer() {
	glGenFramebuffers(1, &_fboId);
	glBindFramebuffer(GL_FRAMEBUFFER, _fboId);
	glDrawBuffer(GL_COLOR_ATTACHMENT0);
}

void Fbo::createTextureAttachment() {
	glGenTextures(1, &_colourTextureId);
	glBindTexture(GL_TEXTURE_2D, _colourTextureId);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, _width, _height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _colourTextureId, 0);
}