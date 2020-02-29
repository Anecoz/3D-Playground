#pragma once
#include <glm/glm.hpp>
#include <string>
#include <vector>

class OBJLoader
{
public:
	OBJLoader();
	~OBJLoader() = default;

	static bool loadFromFile(const std::string& path, std::vector<glm::vec3>& outVerts, std::vector<unsigned>& outIndices, std::vector<glm::vec3>& outNormals);

private:
	std::vector<glm::vec3> fixNormals(const std::vector<unsigned>& vertIndices, const std::vector<unsigned>& normIndices, const std::vector<glm::vec3>& normals, const std::vector<glm::vec3>& verts);
};

