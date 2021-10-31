#include "bola.hpp"

#include <glm/gtx/fast_trigonometry.hpp>
#include <glm/gtx/rotate_vector.hpp>

#include <iostream>
using namespace std;

void Bola::initializeGL(GLuint program) {
  terminateGL();

  m_program = program;
  m_colorLoc = glGetUniformLocation(m_program, "color");
  m_rotationLoc = glGetUniformLocation(m_program, "rotation");
  m_scaleLoc = glGetUniformLocation(m_program, "scale");
  m_translationLoc = glGetUniformLocation(m_program, "translation");

  m_rotation = 0.0f;
  m_translation = glm::vec2(0, 0);
  m_velocity = glm::vec2(0);

  // clang-format off
  std::array<glm::vec2, 4> positions{
      // Bola
      glm::vec2{-01.5f, +01.5}, glm::vec2{-01.5f, -01.5f},
      glm::vec2{+01.5f, -01.5f}, glm::vec2{+01.5f, +01.5f},
    };

  // Normalize
  for (auto &position :positions) {
    position /= glm::vec2{15.0f, 15.0f};
  }

  std::array indices{0, 1, 2,
                     0, 2, 3};
  // clang-format on

  // Generate VBO
  glGenBuffers(1, &m_vbo);
  glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions.data(),
               GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  // Generate EBO
  glGenBuffers(1, &m_ebo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices.data(),
               GL_STATIC_DRAW);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  // Get location of attributes in the program
  GLint positionAttribute{glGetAttribLocation(m_program, "inPosition")};
  

  // Create VAO
  glGenVertexArrays(1, &m_vao);

  // Bind vertex attributes to current VAO
  glBindVertexArray(m_vao);

  glEnableVertexAttribArray(positionAttribute);
  glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
  glVertexAttribPointer(positionAttribute, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);

  // End of binding to current VAO
  glBindVertexArray(0);
}

void Bola::paintGL(const GameData &gameData) {
  if (gameData.m_state != State::Playing) return;

  glUseProgram(m_program);

  glBindVertexArray(m_vao);

  glUniform1f(m_scaleLoc, m_scale);
  glUniform1f(m_rotationLoc, m_rotation);
  glUniform2fv(m_translationLoc, 1, &m_translation.x);

  glUniform4fv(m_colorLoc, 1, &m_color.r);
  glDrawElements(GL_TRIANGLES, 2 * 3, GL_UNSIGNED_INT, nullptr);

  glBindVertexArray(0);

  glUseProgram(0);
}

void Bola::terminateGL() {
  glDeleteBuffers(1, &m_vbo);
  glDeleteBuffers(1, &m_ebo);
  glDeleteVertexArrays(1, &m_vao);
}

void Bola::update(float deltaTime, int &directionX, int &directionY) {
    
    if(directionX == 1){
      setTranslation(glm::vec2(m_translation.x + (deltaTime * (1.2f)), (m_translation.y + (deltaTime * (1.2f) * directionY))));
      setCorBola(glm::vec4(1, 0, 0, 1));
    }
    else if(directionX == -1){
      setTranslation(glm::vec2(m_translation.x - (deltaTime * (1.2f)), (m_translation.y + (deltaTime * (1.2f) * directionY))));
      setCorBola(glm::vec4(0, 0, 1, 1));
    }
}
