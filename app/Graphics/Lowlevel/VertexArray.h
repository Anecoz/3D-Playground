#pragma once

#include <GL/glew.h>

class VertexArray
{
public:
  // Verts
	VertexArray(GLfloat* vertices, GLint count, GLint floatPerVertex);

  // Verts, normals
  VertexArray(GLfloat* vertices, GLint numVerts, GLfloat* normals, GLint numNormals, GLint floatPerVertex);

  // Verts, normals, colors
  VertexArray(GLfloat* vertices, GLint numVerts, GLfloat* normals, GLint numNormals, GLfloat* colors, GLint numColors, GLint floatPerVertex);

  // Verts, texcoords
	VertexArray(GLfloat* texCoords, GLfloat* vertices, GLint vertCount, GLint texCount, GLint floatPerVertex);

	virtual ~VertexArray();

	void virtual draw();

	GLint getVAOID() { return _vao; }
	GLint getCount() { return _count; }

protected:
	GLuint _vao, _vbo, _tbo, _nbo, _cbo;
	GLint _count;

	void bind();
	void unbind();
};

