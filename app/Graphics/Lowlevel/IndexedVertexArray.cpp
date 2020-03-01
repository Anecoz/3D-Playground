#include "IndexedVertexArray.h"

#include "Shader.h"

IndexedVertexArray::IndexedVertexArray(GLfloat* vertices, GLuint* indices, GLint numVerts, GLint indicesCount, GLint floatPerVertex)
  : VertexArray(vertices, numVerts, floatPerVertex)
  , _instanceBo(0)
{
  _count = indicesCount;

	bind();
	glGenBuffers(1, &_ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, _count * sizeof(GLuint), indices, GL_STATIC_DRAW);
	unbind();
}

IndexedVertexArray::IndexedVertexArray(GLfloat* vertices, GLuint* indices, GLfloat* normals, GLint numNormals, GLint numVerts, GLint indicesCount, GLint floatPerVertex)
  : VertexArray(vertices, numVerts, normals, numNormals, floatPerVertex)
  , _instanceBo(0)
{
  _count = indicesCount;

  bind();
	glGenBuffers(1, &_ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, _count * sizeof(GLuint), indices, GL_STATIC_DRAW);
	unbind();
}

IndexedVertexArray::IndexedVertexArray(GLfloat* vertices, GLuint* indices, GLfloat* normals, GLfloat* colors, GLint numColors,
                                       GLint numNormals, GLint numVerts, GLint indicesCount, GLint floatPerVertex, bool instanced)
  : VertexArray(vertices, numVerts, normals, numNormals, colors, numColors, floatPerVertex)
  , _instanceBo(0)
{
  _count = indicesCount;

  bind();
  glGenBuffers(1, &_ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, _count * sizeof(GLuint), indices, GL_STATIC_DRAW);

  if (instanced) {
    glGenBuffers(1, &_instanceBo);
    glBindBuffer(GL_ARRAY_BUFFER, _instanceBo);
    glVertexAttribPointer(Shader::POSOFFSET_ATTRIB_LOC, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(Shader::POSOFFSET_ATTRIB_LOC);
    glVertexAttribDivisor(Shader::POSOFFSET_ATTRIB_LOC, 1);  
  }
	unbind();
}

IndexedVertexArray::IndexedVertexArray(GLfloat* vertices, GLfloat* texCoords, GLuint* indices, GLint numVerts, GLint indicesCount, GLint texCount, GLint floatPerVertex)
	: VertexArray(texCoords, vertices, numVerts, texCount, floatPerVertex)
  , _instanceBo(0)
{
  _count = indicesCount;

	bind();
	glGenBuffers(1, &_ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, _count * sizeof(GLuint), indices, GL_STATIC_DRAW);
	unbind();
}

IndexedVertexArray::~IndexedVertexArray()
{
	glDeleteBuffers(1, &_ibo);
  if (_instanceBo != 0) {
    glDeleteBuffers(1, &_instanceBo);
  }
}

void IndexedVertexArray::updateInstancePosOffsets(const std::vector<float>& posOffsets)
{
  if (_instanceBo == 0) return;

  bind();
  glBindBuffer(GL_ARRAY_BUFFER, _instanceBo);
  glBufferData(GL_ARRAY_BUFFER, posOffsets.size() * sizeof(float), &posOffsets[0], GL_DYNAMIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  unbind();
}

void IndexedVertexArray::drawInstanced(int numInstances)
{
  bind();
  glDrawElementsInstanced(GL_TRIANGLES, _count, GL_UNSIGNED_INT, 0L, numInstances);
  unbind();
}

void IndexedVertexArray::draw()
{
	bind();
	glDrawElements(GL_TRIANGLES, _count, GL_UNSIGNED_INT, 0L);
	unbind();
}

void IndexedVertexArray::drawTesselated()
{
  bind();
  glDrawElements(GL_PATCHES, _count, GL_UNSIGNED_INT, 0L);
  unbind();
}
