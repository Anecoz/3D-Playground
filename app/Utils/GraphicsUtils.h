#pragma once
#include <GL/glew.h>

#include "../Graphics/Lowlevel/IndexedVertexArray.h"

#include <string>

class GraphicsUtils {
public:
  // The caller is responsible for freeing the VAO
  static IndexedVertexArray* createUnitCube();

  // The caller is responsible for freeing the VAO
	static IndexedVertexArray* createModelQuad(float width, float height, float layer);

  // The caller is responsible for freeing the VAO
  static IndexedVertexArray* createGrid(unsigned sizeX, unsigned sizeY);

  // The caller is responsible for freeing the VAO
	static IndexedVertexArray* createSimpleQuad();

  // The caller is responsible for freeing the VAO
  static IndexedVertexArray* loadObj(const std::string& filePath);

	static GLuint loadPNGToTexture(const char* fileName, unsigned* width, unsigned* height);
	static void printError(const char* message);
};
