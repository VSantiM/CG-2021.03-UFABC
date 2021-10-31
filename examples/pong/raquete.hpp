#ifndef RAQUETE_HPP_
#define RAQUETE_HPP_

#include "abcg.hpp"
#include "gamedata.hpp"

//class Bola;
class OpenGLWindow;

class RaqueteP1 {
 public:
  void initializeGL(GLuint program);
  void paintGL(const GameData &gameData);
  void terminateGL();

  void setRotation(float rotation) { m_rotation = rotation; }
  void setTranslation(glm::vec2 translation) { m_translation = translation; }


 private:
  friend OpenGLWindow;

  GLuint m_program{};
  GLint m_translationLoc{};
  GLint m_colorLoc{};
  GLint m_scaleLoc{};
  GLint m_rotationLoc{};

  GLuint m_vao{};
  GLuint m_vbo{};
  GLuint m_ebo{};

  glm::vec4 m_color{1, 0, 0, 1};
  float m_rotation{};
  
  float m_scale{1.0f};
  glm::vec2 m_translation{glm::vec2(0)};

  glm::vec2 m_velocity{glm::vec2(0)};
  glm::vec2 m_position{glm::vec2(0)};

  abcg::ElapsedTimer m_raqueteMoveTimer;
};

#endif