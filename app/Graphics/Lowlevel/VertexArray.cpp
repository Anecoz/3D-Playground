#include "VertexArray.h"
#include "Shader.h"

VertexArray::VertexArray(GLfloat* vertices, GLint count, GLint floatPerVertex) {
  _tbo = 0;
  _cbo = 0;
  _nbo = 0;
  _count = count;

	glGenVertexArrays(1, &_vao);
	glBindVertexArray(_vao);

	glGenBuffers(1, &_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	glBufferData(GL_ARRAY_BUFFER, floatPerVertex * count * sizeof(GLfloat), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(Shader::VERTEX_ATTRIB_LOC, floatPerVertex, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(Shader::VERTEX_ATTRIB_LOC);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

VertexArray::VertexArray(GLfloat* vertices, GLint numVerts, GLfloat* normals, GLint numNormals, GLint floatPerVertex)
{
  _tbo = 0;
  _cbo = 0;
  _nbo = 0;
  _count = numVerts;

  glGenVertexArrays(1, &_vao);
	glBindVertexArray(_vao);

	glGenBuffers(1, &_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	glBufferData(GL_ARRAY_BUFFER, numVerts * sizeof(GLfloat), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(Shader::VERTEX_ATTRIB_LOC, floatPerVertex, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(Shader::VERTEX_ATTRIB_LOC);

	glGenBuffers(1, &_nbo);
	glBindBuffer(GL_ARRAY_BUFFER, _nbo);
	glBufferData(GL_ARRAY_BUFFER, numNormals * sizeof(GLfloat), normals, GL_STATIC_DRAW);
	glVertexAttribPointer(Shader::NORMAL_ATTRIB_LOC, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(Shader::NORMAL_ATTRIB_LOC);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

VertexArray::VertexArray(GLfloat* vertices, GLint numVerts, GLfloat* normals, GLint numNormals, GLfloat* colors, GLint numColors, GLint floatPerVertex)
{
  _tbo = 0;
  _cbo = 0;
  _nbo = 0;
  _count = numVerts;

  glGenVertexArrays(1, &_vao);
	glBindVertexArray(_vao);

	glGenBuffers(1, &_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	glBufferData(GL_ARRAY_BUFFER, numVerts * sizeof(GLfloat), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(Shader::VERTEX_ATTRIB_LOC, floatPerVertex, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(Shader::VERTEX_ATTRIB_LOC);

	glGenBuffers(1, &_nbo);
	glBindBuffer(GL_ARRAY_BUFFER, _nbo);
	glBufferData(GL_ARRAY_BUFFER, numNormals * sizeof(GLfloat), normals, GL_STATIC_DRAW);
	glVertexAttribPointer(Shader::NORMAL_ATTRIB_LOC, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(Shader::NORMAL_ATTRIB_LOC);

  glGenBuffers(1, &_cbo);
	glBindBuffer(GL_ARRAY_BUFFER, _cbo);
	glBufferData(GL_ARRAY_BUFFER, numColors * sizeof(GLfloat), colors, GL_STATIC_DRAW);
	glVertexAttribPointer(Shader::COLOR_ATTRIB_LOC, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(Shader::COLOR_ATTRIB_LOC);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

/*VertexArray::VertexArray(GLfloat* texCoords, GLfloat* vertices, GLint numVerts, GLint numTex, GLint floatPerVertex) {
  _tbo = 0;
  _cbo = 0;
  _nbo = 0;
  _count = numVerts;

	glGenVertexArrays(1, &_vao);
	glBindVertexArray(_vao);

	glGenBuffers(1, &_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	glBufferData(GL_ARRAY_BUFFER, floatPerVertex * numVerts * sizeof(GLfloat), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(Shader::VERTEX_ATTRIB_LOC, floatPerVertex, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(Shader::VERTEX_ATTRIB_LOC);

	glGenBuffers(1, &_tbo);
	glBindBuffer(GL_ARRAY_BUFFER, _tbo);
	glBufferData(GL_ARRAY_BUFFER, 2 * numTex * sizeof(GLfloat), texCoords, GL_STATIC_DRAW);
	glVertexAttribPointer(Shader::TEX_ATTRIB_LOC, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(Shader::TEX_ATTRIB_LOC);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}*/

void VertexArray::bind() {
	glBindVertexArray(_vao);
}

void VertexArray::unbind() {
	glBindVertexArray(0);
}

void VertexArray::draw() {
	bind();
	glDrawArrays(GL_TRIANGLES, 0, _count);
	unbind();
}

VertexArray::~VertexArray() {
	glDeleteVertexArrays(1, &_vao);
	glDeleteBuffers(1, &_vbo);
	if (_tbo != 0)
		glDeleteBuffers(1, &_tbo);
  if (_nbo != 0)
    glDeleteBuffers(1, &_nbo);
  if (_cbo != 0)
    glDeleteBuffers(1, &_cbo);
}
