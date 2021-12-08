#ifndef SISTEMA_HPP_
#define SISTEMA_HPP_

#include "abcg.hpp"
#include <glm/vec3.hpp>
#include <vector>

class OpenGLWindow;
class Camera;

class Sistema {
 public:
  void iniciarSistema(std::string path);
  bool hasFinished(glm::vec3 position);

 private:
  friend OpenGLWindow;
  friend Camera;

  std::vector<std::vector<char>> m_sistemaMatrix;
  
  glm::vec3 m_startPosition;
  glm::vec3 m_endPosition;

  glm::vec3 getPositionFromFile(std::ifstream &file);
};

#endif