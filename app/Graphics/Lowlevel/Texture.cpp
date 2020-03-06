#include "Texture.h"
#include "../../Utils/GraphicsUtils.h"

Texture::Texture(unsigned width, unsigned height, GLuint id, GLint type)
  : _width(width)
  , _height(height)
  , _texId(id)
  , _type(type)
{}

Texture::Texture(const std::string& path)
{
	_texId = GraphicsUtils::loadPNGToTexture(path.c_str(), &_width, &_height);
}

Texture::~Texture()
{
  glDeleteTextures(1, &_texId);
}

void Texture::bind() const
{
	glBindTexture(_type, _texId);
}

void Texture::unbind() const
{
	glBindTexture(_type, 0);
}