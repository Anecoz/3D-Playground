#include "DrawableCube.h"

#include "Camera.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include "../tiny_obj_loader.h"
#include "../Utils/GraphicsUtils.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/transform.hpp>

DrawableCube::DrawableCube()
  : _shader("/home/christoph/dev/3D-Playground/app/Graphics/cube.vert", "/home/christoph/dev/3D-Playground/app/Graphics/cube.frag")
  , _cubeMesh(GraphicsUtils::createUnitCube())
  //, _cubeMesh(GraphicsUtils::loadObj("/home/christoph/dev/3D-Playground/app/assets/lowpolytree.obj"))
  , _modelMatrix(glm::scale(glm::vec3(100.0)))
{
  tinyobj::attrib_t attrib;
  std::vector<tinyobj::shape_t> shapes;
  std::vector<tinyobj::material_t> materials;
  std::string warn, err;
  tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err,
  "/home/christoph/dev/3D-Playground/app/assets/lowpolytree.obj",
  "/home/christoph/dev/3D-Playground/app/assets/");
  printf("Warn: %s, err: %s\n", warn.c_str(), err.c_str());

  // NOTE:
  // materials contains 2 materials in this case, with info on how to shade
  // shapes.mesh.material_ids has a "per-index" id that maps to the materials.
  // Use this to know how to shade each vertex/fragment in shader?
  // Use another attribute location for material parameters, rgb, shininess etc. These are then indexed like the vertices and normals (duplicated)

  // Vertices are in attrib.
  // Indices are in shapes
  // Convert indices and duplicate normals (to match vertex indices)
  std::vector<int> indices;
  std::vector<float> normals;
  std::vector<float> colors;
  colors.resize(attrib.vertices.size());
  normals.resize(attrib.vertices.size());
  for (int i = 0; i < shapes[0].mesh.indices.size(); ++i) {
    tinyobj::index_t idx = shapes[0].mesh.indices[i];
    indices.emplace_back(idx.vertex_index);

    normals[3 * idx.vertex_index + 0] = attrib.normals[3 * idx.normal_index + 0];
    normals[3 * idx.vertex_index + 1] = attrib.normals[3 * idx.normal_index + 1];
    normals[3 * idx.vertex_index + 2] = attrib.normals[3 * idx.normal_index + 2];

    int materialId = shapes[0].mesh.material_ids[i/3];    

    colors[3 * idx.vertex_index + 0] = materials[materialId].diffuse[0];
    colors[3 * idx.vertex_index + 1] = materials[materialId].diffuse[1];
    colors[3 * idx.vertex_index + 2] = materials[materialId].diffuse[2];
  }

  _cubeMesh = std::unique_ptr<IndexedVertexArray>(new IndexedVertexArray(
    (GLfloat*)&attrib.vertices[0],
    (GLuint*)&indices[0],
    (GLfloat*)&normals[0],
    (GLfloat*)&colors[0],
    colors.size(),
    normals.size(),
    attrib.vertices.size(),
    indices.size(),
    3));
}

void DrawableCube::draw(const Camera& camera)
{
  _shader.bind();
  _shader.uploadMatrix(_modelMatrix, "modelMatrix");
  _shader.uploadMatrix(camera.getCamMatrix(), "camMatrix");
  _shader.uploadMatrix(camera.getProjection(), "projMatrix");
  _cubeMesh->draw();
  _shader.unbind();
}