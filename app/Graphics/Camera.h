#pragma once

#include "Frustum.h"

#include <glm/glm.hpp>

class Camera
{
public:
	Camera(const glm::vec3& initialPosition);
	~Camera() = default;

	void update(double delta);

	void setPosition(const glm::vec3& posIn) { _position = posIn; }
	glm::vec3 getPosition() const { return _position; }
  glm::mat4 getProjection() const { return _projection; }
  glm::mat4 getCamMatrix() const { return _cameraMatrix; }
  glm::mat4 getCombined() const { return _projection * _cameraMatrix; }

  bool insideFrustum(const glm::vec3& point) const;
  bool insideFrustum(const Box3D& box) const;

private:
  void freelookUpdate(double delta);
  void handleFreelookInput(double delta);

  Frustum _frustum;

  glm::vec2 _prevFreelookMousePos;
  bool _firstMouse;
  double _yaw;
  double _pitch;
  double _roll;

  glm::mat4 _projection;
  glm::mat4 _cameraMatrix;
  glm::vec3 _position;
  glm::vec3 _forward;
  glm::vec3 _right;
  glm::vec3 _up;
  const double _speed = 5.0;
  double _sensitivity = 0.005;
};

