#include "Camera.h"

#include "../Input/KeyInput.h"
#include "../Input/MousePosInput.h"

#include <stdio.h>
#include <iostream>
#include <string>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/transform.hpp>

Camera::Camera(const glm::vec3& initialPosition)
  : _yaw(0.0)
  , _pitch(0.0)
  , _roll(0.0)
  , _firstMouse(true)
  , _projection(glm::perspective(glm::radians(45.0), 16.0/9.0, 0.1, 1000.0))  
  , _position(initialPosition)
  , _forward(0.0f, 0.0f, 0.0f)
  , _cameraMatrix(glm::lookAt(_position, _forward, _up))
{}

bool Camera::insideFrustum(const glm::vec3& point) const
{
  auto inside = _frustum.isInside(point);
  if (inside == Frustum::Visibility::Partially || inside == Frustum::Visibility::Completly) {
    return true;
  }
  return false;
}

bool Camera::insideFrustum(const Box3D& box) const
{
  auto inside = _frustum.isInside(box);
  if (inside == Frustum::Visibility::Partially || inside == Frustum::Visibility::Completly) {
    return true;
  }

  return false;
}

void Camera::update(double delta)
{
  if (_firstMouse) {
    glm::vec2 m = MousePosInput::getPosition();
    _prevFreelookMousePos.x = m.x;
    _prevFreelookMousePos.y = m.y;
    _firstMouse = false;
  }

  // Check input and move camera
  freelookUpdate(delta);

  _frustum.transform(_projection, _cameraMatrix);
  //std::cout << "Camera at: " << _position.x << ", " << _position.y << ", " << _position.z << std::endl;
}

void Camera::freelookUpdate(double delta)
{
  handleFreelookInput(delta);

  glm::mat4 view = glm::translate(_position);
  view *= glm::rotate((float)_yaw, glm::vec3(0, 1, 0));
  view *= glm::rotate((float)_pitch, glm::vec3(1, 0, 0));
  _cameraMatrix = glm::inverse(view);

  _right = glm::normalize(view[0]);
  _up = glm::normalize(view[1]);
  _forward = -glm::normalize(view[2]);
}

void Camera::handleFreelookInput(double delta)
{
  GLfloat speedModifier = _speed;

  if (KeyInput::isKeyDown(GLFW_KEY_SPACE))
    speedModifier += 3.0;
  if (KeyInput::isKeyDown(GLFW_KEY_LEFT_SHIFT))
    speedModifier += 30.0;
  if (KeyInput::isKeyDown(GLFW_KEY_LEFT_ALT))
    speedModifier += 300.0;

  GLfloat speed = (speedModifier*delta);

  glm::vec3 movement(0.0, 0.0, 0.0);

  if (KeyInput::isKeyDown(GLFW_KEY_W)) {
    movement += _forward * speed;
  }
  if (KeyInput::isKeyDown(GLFW_KEY_S)) {
    movement -= _forward * speed;
  }
  if (KeyInput::isKeyDown(GLFW_KEY_A)) {
    movement -= _right * speed;
  }
  if (KeyInput::isKeyDown(GLFW_KEY_D)) {
    movement += _right * speed;
  }

  _position += movement;

  glm::vec2 mouseDelta = MousePosInput::getPosition() - _prevFreelookMousePos;
  _prevFreelookMousePos = MousePosInput::getPosition();

  _yaw -= mouseDelta.x * _sensitivity;
  _pitch -= mouseDelta.y * _sensitivity;

  // Yaw and pitch limits
  if (_yaw >= glm::radians(360.0)) {
    _yaw = glm::radians(0.0);
  }
  else if (_yaw < glm::radians(0.0)) {
    _yaw = glm::radians(360.0) - _yaw;
  }
  if (_pitch > glm::radians(85.0)) {
    _pitch = glm::radians(85.0);
  }
  else if (_pitch < glm::radians(-85.0)) {
    _pitch = glm::radians(-85.0);
  }
}
