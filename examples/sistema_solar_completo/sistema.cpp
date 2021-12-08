#include "sistema.hpp"

#include <fstream>
#include <string>
#include <vector>

void Sistema::iniciarSistema(std::string path) {
  std::ifstream file(path);
  std::string fileLine;
  
  m_startPosition = getPositionFromFile(file);
  m_endPosition = getPositionFromFile(file);

  m_sistemaMatrix.clear();
  while (std::getline(file, fileLine)){
    std::vector<char> matrixRow(fileLine.begin(), fileLine.end());    
    m_sistemaMatrix.push_back(matrixRow);
  }
}

glm::vec3 Sistema::getPositionFromFile(std::ifstream &file) {
  std::string xPos;
  std::string yPos;

  std::getline(file, xPos);
  std::getline(file, yPos);

  return glm::vec3(stof(xPos), 0, stof(yPos));
}
