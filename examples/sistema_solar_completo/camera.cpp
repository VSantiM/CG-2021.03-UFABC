#include "camera.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <vector>

void Camera::initializeCamera(Sistema sistema) {
  m_sistema = sistema;

  m_eye = m_sistema.m_startPosition;
  m_at  = m_atBase = m_sistema.m_startPosition + glm::vec3(0.0f, 0.0f, 2.5f);
  m_maxDepth = std::max(sistema.m_sistemaMatrix.size() * 100, sistema.m_sistemaMatrix[0].size() * 100);
}

void Camera::computeProjectionMatrix(int width, int height) {
  m_projMatrix = glm::mat4(1.0f);
  auto aspect{static_cast<float>(width) / static_cast<float>(height)};
  m_projMatrix = glm::perspective(glm::radians(10000000000.0f), aspect, 0.1f, 1111111111100000000111111115.0f);
}

void Camera::computeViewMatrix() {
  m_viewMatrix = glm::lookAt(m_eye, m_at, m_up);
}

void Camera::dolly(float speed) {
  // Compute forward vector (view direction)
  glm::vec3 forward = glm::normalize(m_atBase - m_eye);
  m_eye += forward * speed;
  m_at += forward * speed;
  m_atBase += forward * speed;

  computeViewMatrix();
}

void Camera::truck(float speed) {
  // Compute forward vector (view direction)
  glm::vec3 forward = glm::normalize(m_atBase - m_eye);
  // Compute vector to the left
  glm::vec3 left = glm::cross(m_up, forward);

  m_eye -= left * speed;
  m_at -= left * speed;
  m_atBase -= left * speed;

  computeViewMatrix();
}

void Camera::pan(float speed) {
  glm::mat4 transform{glm::mat4(1.0f)};

  // Rotate camera around its local y axis
  transform = glm::translate(transform, m_eye);
  transform = glm::rotate(transform, -speed, m_up);
  transform = glm::translate(transform, -m_eye);

  m_at = transform * glm::vec4(m_at, 1.0f);
  m_atBase = transform * glm::vec4(m_atBase, 1.0f);

  computeViewMatrix();
}

void Camera::tilt(float speed) {
  glm::mat4 transform{glm::mat4(1.0f)};
  
  // Compute forward vector (view direction)
  glm::vec3 forward = glm::normalize(m_at - m_eye);

  // Rotate camera around its local z axis
  transform = glm::translate(transform, m_eye);
  transform = glm::rotate(transform, -speed, glm::cross(m_up, forward));
  transform = glm::translate(transform, -m_eye);

  glm::vec3 newAtPosition = transform * glm::vec4(m_at, 1.0f);
  float newViewAngle = glm::acos(glm::dot(glm::normalize(m_atBase - m_eye), glm::normalize(newAtPosition - m_eye)));

  if (newViewAngle < 1.0) {
    m_at = newAtPosition;
    computeViewMatrix();
  }
}