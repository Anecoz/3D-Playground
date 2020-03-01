#pragma once

#include <GL/glew.h>
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

  // Verts, normals, colors, indices
  IndexedVertexArray(GLfloat* vertices, GLuint* indices, GLfloat* normals, GLfloat* colors, GLint numColors,
                     GLint numNormals, GLint numVerts, GLint indicesCount, GLint floatPerVertex, bool instanced = false);

  // Verts, texcoords, indices
	IndexedVertexArray(GLfloat* vertices, GLfloat* texCoords, GLuint* indices, GLint numVerts, GLint indicesCount, GLint texCount, GLint floatPerVertex);

	virtual ~IndexedVertexArray();

  void updateInstancePosOffsets(const std::vector<float>& posOffsets);
	
	void draw() override;
  void drawTesselated();
  void drawInstanced(int numInstances);
	
private:
  GLuint _instanceBo;
	GLuint _ibo;
};

