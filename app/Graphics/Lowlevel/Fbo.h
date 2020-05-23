#pragma once

#include <GL/glew.h>

#include <cstddef>
#include <vector>

class Fbo
{
public:
  Fbo(int width, int height, bool color, int numDepth);
  ~Fbo();

  void bind(std::size_t depthIndex);
  void unbind();

  GLuint getColourTextureId() { return _colourTextureId; }
  GLuint getDepthTextureId(std::size_t index) { return _depthTextureIds[index]; }

private:
  int _width;
  int _height;
  bool _color;
  int _numDepth;
  GLuint _fboId, _colourTextureId;

  std::vector<GLuint> _depthTextureIds;

  void initialiseFramebuffer();
  void createFramebuffer();
  void createTextureAttachment();
  void createDepthBufferAttachment();
};

