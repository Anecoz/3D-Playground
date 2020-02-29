#include "OBJLoader.h"
#include <iostream>
#include <algorithm>

OBJLoader::OBJLoader()
{
}

bool OBJLoader::loadFromFile(const std::string& path, std::vector<glm::vec3>& outVerts, std::vector<unsigned>& outIndices, std::vector<glm::vec3>& outNormals) {
	FILE * file = fopen(path.c_str(), "r");
	if (file == NULL) {
		std::cerr << "Impossible to open the file " << path << "!" << std::endl;
		return false;
	}

	std::vector<glm::vec3> tmpVertices;	
	std::vector<glm::vec3> tmpNormals;
	std::vector<unsigned> vertexIndices;
	//std::vector<unsigned> normalIndices;

	while (1) {

		char lineHeader[128];
		// read the first word of the line
		int res = fscanf(file, "%s", lineHeader);        
		if (res == EOF)
			break; // EOF = End Of File. Quit the loop.

		if (strcmp(lineHeader, "v") == 0) {
			glm::vec3 vertex;
			fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
			tmpVertices.push_back(vertex);
		}
		else if (strcmp(lineHeader, "vt") == 0) {
		}
		/*else if (strcmp(lineHeader, "vn") == 0) {
			glm::vec3 normal;
			fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
			tmpNormals.push_back(normal);
		}*/
		else if (strcmp(lineHeader, "f") == 0) {
			unsigned int vertexIndex[3], normalIndex[3];
			int matches = fscanf(file, "%d//%d %d//%d %d//%d\n", &vertexIndex[0], &normalIndex[0], &vertexIndex[1], &normalIndex[1], &vertexIndex[2], &normalIndex[2]);
			if (matches != 6) {
				printf("File can't be read by our simple parser : ( Try exporting with other options\n");
				return false;
			}
			vertexIndices.push_back(vertexIndex[0] - 1);
			vertexIndices.push_back(vertexIndex[1] - 1);
			vertexIndices.push_back(vertexIndex[2] - 1);
			//normalIndices.push_back(normalIndex[0] - 1);
			//normalIndices.push_back(normalIndex[1] - 1);
			//normalIndices.push_back(normalIndex[2] - 1);
		}
	}
	
	//tmpNormals = fixNormals(vertexIndices, normalIndices, tmpNormals, tmpVertices);
	outVerts = tmpVertices;
	outIndices = vertexIndices;
	outNormals = tmpNormals;
	return true;
}

std::vector<glm::vec3> OBJLoader::fixNormals(const std::vector<unsigned>& vertIndices, const std::vector<unsigned>& normIndices, const std::vector<glm::vec3>& normals, const std::vector<glm::vec3>& verts) {
	// Need new normal array with same size as vertices...
	std::vector<glm::vec3> newNormals;

	for (unsigned i = 0; i < verts.size(); i++) {
		// Find index of VERT INDEX ARRAY of this vert (first occurence is sufficient)
		int index = std::find(vertIndices.begin(), vertIndices.end(), i) - vertIndices.begin();
		newNormals.push_back(normals[normIndices[index]]);
	}

	return newNormals;
}