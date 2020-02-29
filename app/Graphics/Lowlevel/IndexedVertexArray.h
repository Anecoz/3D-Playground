#pragma once

#include <glm/glm.hpp>
#include <GL/glew.h>
#include "VertexArray.h"

#include <vector>

class IndexedVertexArray : public VertexArray
{
public:
	IndexedVertexArray(GLfloat* vertices, GLuint* indices, GLint numVerts, GLint indicesCount, GLint floatPerVertex);
  IndexedVertexArray(GLfloat* vertices, GLuint* indices, GLfloat* normals, GLint numNormals, GLint numVerts, GLint indicesCount, GLint floatPerVertex);
  //IndexedVertexArray(const std::vector<glm::vec3>& verts, const std::vector<unsigned int>& indices, GLint floatPerVertex);
	IndexedVertexArray(GLfloat* vertices, GLfloat* texCoords, GLuint* indices, GLint numVerts, GLint indicesCount, GLint texCount, GLint floatPerVertex);
	virtual ~IndexedVertexArray();
	
	void draw() override;
  void drawTesselated();
	
private:
	GLuint _ibo;
};

