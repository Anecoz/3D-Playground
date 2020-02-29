#include "IndexedVertexArray.h"

IndexedVertexArray::IndexedVertexArray(GLfloat* vertices, GLuint* indices, GLint numVerts, GLint indicesCount, GLint floatPerVertex)
  : VertexArray(vertices, numVerts, floatPerVertex) {
  _count = indicesCount;

	bind();
	glGenBuffers(1, &_ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, _count * sizeof(GLuint), indices, GL_STATIC_DRAW);
	unbind();
}

IndexedVertexArray::IndexedVertexArray(GLfloat* vertices, GLuint* indices, GLfloat* normals, GLint numNormals, GLint numVerts, GLint indicesCount, GLint floatPerVertex)
  : VertexArray(vertices, numVerts, normals, numNormals, floatPerVertex)
{
  _count = indicesCount;

  bind();
	glGenBuffers(1, &_ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, _count * sizeof(GLuint), indices, GL_STATIC_DRAW);
	unbind();
}

IndexedVertexArray::IndexedVertexArray(GLfloat* vertices, GLuint* indices, GLfloat* normals, GLfloat* colors, GLint numColors,
                                       GLint numNormals, GLint numVerts, GLint indicesCount, GLint floatPerVertex)
  : VertexArray(vertices, numVerts, normals, numNormals, colors, numColors, floatPerVertex)
{
  _count = indicesCount;

  bind();
  glGenBuffers(1, &_ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, _count * sizeof(GLuint), indices, GL_STATIC_DRAW);
	unbind();
}

/*IndexedVertexArray::IndexedVertexArray(const std::vector<glm::vec3>& verts, const std::vector<unsigned int>& indices, GLint floatPerVertex)
{
}*/

/*IndexedVertexArray::IndexedVertexArray(GLfloat* vertices, GLfloat* texCoords, GLuint* indices, GLint numVerts, GLint indicesCount, GLint texCount, GLint floatPerVertex)
	: VertexArray(texCoords, vertices, numVerts, texCount, floatPerVertex) {
  _count = indicesCount;

	bind();
	glGenBuffers(1, &_ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, _count * sizeof(GLuint), indices, GL_STATIC_DRAW);
	unbind();
}*/

IndexedVertexArray::~IndexedVertexArray() {
	glDeleteBuffers(1, &_ibo);
}

void IndexedVertexArray::draw() {
	bind();
	glDrawElements(GL_TRIANGLES, _count, GL_UNSIGNED_INT, 0L);
	unbind();
}

void IndexedVertexArray::drawTesselated() {
  bind();
  glDrawElements(GL_PATCHES, _count, GL_UNSIGNED_INT, 0L);
  unbind();
}
