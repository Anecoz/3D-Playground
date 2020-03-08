#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "VertexArray.h"

#include <vector>

class IndexedVertexArray : public VertexArray
{
public:
  // Verts, indices
	IndexedVertexArray(GLfloat* vertices, GLuint* indices, GLint numVerts, GLint indicesCount, GLint floatPerVertex);

  // Verts, normals, indices
  IndexedVertexArray(GLfloat* vertices, GLuint* indices, GLfloat* normals,
                     GLint numNormals, GLint numVerts, GLint indicesCount, GLint floatPerVertex);

  // Verts, colors, indices
  IndexedVertexArray(GLfloat* vertices, GLuint* indices, GLfloat* colors, GLint numColors,
                     GLint numVerts, GLint indicesCount, GLint floatPerVertex, bool instanced = false);

  // Verts, normals, colors, indices
  IndexedVertexArray(GLfloat* vertices, GLuint* indices, GLfloat* normals, GLfloat* colors, GLint numColors,
                     GLint numNormals, GLint numVerts, GLint indicesCount, GLint floatPerVertex, bool instanced = false);

  // Verts, texcoords, indices
	IndexedVertexArray(GLfloat* vertices, GLfloat* texCoords, GLuint* indices, GLint numVerts, GLint indicesCount, GLint texCount, GLint floatPerVertex);

	virtual ~IndexedVertexArray();

  void updateInstanceMatrices(const std::vector<glm::mat4>& modelMatrices);
	
	void draw() override;
  void drawTesselated();
  void drawInstanced(GLsizei numInstances);
	
private:
  GLuint _instanceBo;
	GLuint _ibo;
};

