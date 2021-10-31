#ifndef OPENGLWINDOW_HPP_
#define OPENGLWINDOW_HPP_

#include <imgui.h>

#include <random>

#include "abcg.hpp"
#include "raquete.hpp"
#include "raquete_player2.hpp"
#include "bola.hpp"

class OpenGLWindow : public abcg::OpenGLWindow {
 protected:
  void handleEvent(SDL_Event& event) override;
  void initializeGL() override;
  void paintGL() override;
  void paintUI() override;
  void resizeGL(int width, int height) override;
  void terminateGL() override;

 private:
  GLuint m_raqueteProgram{};
  GLuint m_raqueteProgram2{};
  GLuint m_bolaProgram{};

  int m_viewportWidth{};
  int m_viewportHeight{};

  GameData m_gameData;

  RaqueteP1 m_raquete;
  RaqueteP2 m_raquete2;
  Bola m_bola;
  int directionX = 1; //-1 esquerda 1 direita
  int directionY = 0;
  int player1Score = 0;
  int player2Score = 0;

  abcg::ElapsedTimer m_restartWaitTimer;

  ImFont* m_font{};

  std::default_random_engine m_randomEngine;

  void checkCollisions();
  void checkWinCondition();

  void restart();
  void update();
};

#endif