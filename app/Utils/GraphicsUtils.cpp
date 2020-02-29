#include "GraphicsUtils.h"

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "../LodePng/lodepng.h"
#include "OBJLoader.h"

#include <stdio.h>
#include <iostream>
#include <iomanip>
#include <vector>

IndexedVertexArray* GraphicsUtils::createUnitCube()
{
  GLfloat vertices[] = {
    // front
    -1.0, -1.0,  1.0,
    1.0, -1.0,  1.0,
    1.0,  1.0,  1.0,
    -1.0,  1.0,  1.0,
    // back
    -1.0, -1.0, -1.0,
    1.0, -1.0, -1.0,
    1.0,  1.0, -1.0,
    -1.0,  1.0, -1.0,
  };

  GLuint indices[] = {
    // front
    0, 1, 2,
    2, 3, 0,
    // top
    1, 5, 6,
    6, 2, 1,
    // back
    7, 6, 5,
    5, 4, 7,
    // bottom
    4, 0, 3,
    3, 7, 4,
    // left
    4, 5, 1,
    1, 0, 4,
    // right
    3, 2, 6,
    6, 7, 3,
  };

  return new IndexedVertexArray(vertices, indices, 8 * 3, 6 * 2 * 3, 3);
}

IndexedVertexArray* GraphicsUtils::createModelQuad(float width, float height, float layer) {
	/*GLfloat* vertices = new GLfloat[12];
	vertices[0] = 0.0f; vertices[1] = 0.0f; vertices[2] = layer;	
	vertices[3] = 0.0f; vertices[4] = height; vertices[5] = layer;	
	vertices[6] = width; vertices[7] = 0.0f; vertices[8] = layer;
	vertices[9] = width; vertices[10] = height; vertices[11] = layer;

	GLfloat* texCoords = new GLfloat[8];
	texCoords[0] = 0.0f; texCoords[1] = 1.0f;
	texCoords[2] = 0.0f; texCoords[3] = 0.0f;
	texCoords[4] = 1.0f; texCoords[5] = 1.0f;
	texCoords[6] = 1.0f; texCoords[7] = 0.0f;

	GLuint* indices = new GLuint[6];
	indices[0] = 0; indices[1] = 1; indices[2] = 2;
	indices[3] = 2; indices[4] = 1; indices[5] = 3;
	
	return new IndexedVertexArray(vertices, texCoords, indices, 4, 6, 4, 3);*/
  return nullptr;
}

IndexedVertexArray* GraphicsUtils::createSimpleQuad() {
	GLfloat* vertices = new GLfloat[8];
	vertices[0] = 0.0f;
	vertices[1] = 0.0f;
	vertices[2] = 0.0f;
	vertices[3] = 1.0f;
	vertices[4] = 1.0f;
	vertices[5] = 0.0f;
	vertices[6] = 1.0f;
	vertices[7] = 1.0f;

	GLuint* indices = new GLuint[6];
	indices[0] = 0;
	indices[1] = 1;
	indices[2] = 2;
	indices[3] = 2;
	indices[4] = 1;
	indices[5] = 3;

	return new IndexedVertexArray(vertices, indices, 4, 6, 2);
}

static GLfloat* g_lastGridVert = nullptr;
static unsigned* g_lastGridIndices = nullptr;
static unsigned g_lastGridX = 0;
static unsigned g_lastGridY = 0;

IndexedVertexArray* GraphicsUtils::createGrid(unsigned sizeX, unsigned sizeY)
{
  double start = glfwGetTime();
  unsigned numVerts = (sizeX + 1) * (sizeY + 1) * 3;
  unsigned numIndices = sizeX * sizeY * 6;

  if (g_lastGridIndices && g_lastGridVert) {
    if (g_lastGridX == sizeX && g_lastGridY == sizeY) {
      auto ret = new IndexedVertexArray(g_lastGridVert, g_lastGridIndices, numVerts, numIndices, 3);
      double end = glfwGetTime();
      printf("createGrid took %lf ms\n", (end - start) * 1000.0);
      return ret;
    }
  }

  GLfloat* verts = new GLfloat[numVerts];
  unsigned* indices = new unsigned[numIndices];

  for (unsigned y = 0; y <= sizeY; ++y) {
    for (unsigned x = 0; x <= sizeX; ++x) {
      unsigned offset = 3 * (y * (sizeX + 1));
      verts[offset + (x * 3) + 0] = (GLfloat)x;
      verts[offset + (x * 3) + 1] = 0.0f;
      verts[offset + (x * 3) + 2] = (GLfloat)y;
    }
  }

  for (unsigned y = 0; y < sizeY; ++y) {
    for (unsigned x = 0; x < sizeX; ++x) {
      indices[6 * sizeX * y + (x * 6) + 0] = y * (sizeX + 1) + x;
      indices[6 * sizeX * y + (x * 6) + 1] = (y + 1) * (sizeX + 1) + (x + 1);
      indices[6 * sizeX * y + (x * 6) + 2] = y * (sizeX + 1) + (x + 1);
      indices[6 * sizeX * y + (x * 6) + 3] = y * (sizeX + 1) + x;
      indices[6 * sizeX * y + (x * 6) + 4] = (y + 1) * (sizeX + 1) + x;
      indices[6 * sizeX * y + (x * 6) + 5] = (y + 1) * (sizeX + 1) + (x + 1);
    }
  }

  g_lastGridIndices = indices;
  g_lastGridVert = verts;
  g_lastGridX = sizeX;
  g_lastGridY = sizeY;

  auto ret = new IndexedVertexArray(verts, indices, numVerts, numIndices, 3);
  double end = glfwGetTime();
  printf("createGrid took %lf ms\n", (end - start) * 1000.0);
  return ret;
}

IndexedVertexArray* GraphicsUtils::loadObj(const std::string& filePath)
{
  std::vector<glm::vec3> verts;
  std::vector<glm::vec3> normals;
  std::vector<unsigned int> indices;

  if (OBJLoader::loadFromFile(filePath, verts, indices, normals)) {
    // Convert to float array
    GLfloat* vertArr = new GLfloat[verts.size() * 3];
    for (int i = 0; i < verts.size(); i++) {
      vertArr[i * 3 + 0] = verts[i].x;
      vertArr[i * 3 + 1] = verts[i].y;
      vertArr[i * 3 + 2] = verts[i].z;
    }
    return new IndexedVertexArray(vertArr, &indices[0], verts.size(), indices.size(), 3);
  }
  return nullptr;
}

GLuint GraphicsUtils::loadPNGToTexture(const char* fileName, unsigned* width, unsigned* height) {
	GLuint texId;
	//unsigned width, height;
	std::vector<unsigned char> image;
	unsigned error = lodepng::decode(image, *width, *height, fileName);

	if (error != 0) {
    std::cerr << "TEXTURE ERROR: " << error << ": " << lodepng_error_text(error) << std::endl;
    std::cerr << "File was : " << fileName << std::endl;
		return 0;
	}

	glGenTextures(1, &texId);
	glBindTexture(GL_TEXTURE_2D, texId);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, *width, *height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &image[0]);
	glBindTexture(GL_TEXTURE_2D, 0);

	return texId;
}

void GraphicsUtils::printError(const char* message) {
	int error;
	if ((error = glGetError()) != GL_NO_ERROR) {
    std::cerr << "error: " << message << ", code: " << std::hex << error << std::endl;
	}
}