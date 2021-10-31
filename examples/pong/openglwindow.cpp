#include "openglwindow.hpp"

#include <iostream>
#include "glm/ext.hpp"
#include <cstdio>
using namespace std;

#include <imgui.h>

#include "abcg.hpp"

void OpenGLWindow::handleEvent(SDL_Event &event) {

  if (event.type == SDL_KEYDOWN) {
    if (event.key.keysym.sym == SDLK_w && m_raquete.m_translation.y < 0.8f){      
      m_gameData.m_input.set(static_cast<size_t>(Input::Up));
    }
    if (event.key.keysym.sym == SDLK_s && m_raquete.m_translation.y > -0.8f){
      m_gameData.m_input.set(static_cast<size_t>(Input::Down));
    }
    if (event.key.keysym.sym == SDLK_UP && m_raquete2.m_translation.y < 0.8f){
      m_gameData.m_input.set(static_cast<size_t>(Input::UpR));
    }
    if (event.key.keysym.sym == SDLK_DOWN && m_raquete2.m_translation.y > -0.8f){
      m_gameData.m_input.set(static_cast<size_t>(Input::DownR));
    }
  }

  if (event.type == SDL_KEYUP) {
    if (event.key.keysym.sym == SDLK_w)
      m_gameData.m_input.reset(static_cast<size_t>(Input::Up));
    if (event.key.keysym.sym == SDLK_s)
      m_gameData.m_input.reset(static_cast<size_t>(Input::Down));
    if (event.key.keysym.sym == SDLK_UP)
      m_gameData.m_input.reset(static_cast<size_t>(Input::UpR));
    if (event.key.keysym.sym == SDLK_DOWN)
      m_gameData.m_input.reset(static_cast<size_t>(Input::DownR));
  }
}

void OpenGLWindow::initializeGL() {

  ImGuiIO &io{ImGui::GetIO()};
  auto filename{getAssetsPath() + "BebasNeue-Regular.ttf"};
  m_font = io.Fonts->AddFontFromFileTTF(filename.c_str(), 60.0f);
  if (m_font == nullptr) {
    throw abcg::Exception{abcg::Exception::Runtime("Cannot load font file")};
  }

  m_raqueteProgram = createProgramFromFile(getAssetsPath() + "raquete.vert",
                                         getAssetsPath() + "raquete.frag");
                                         
  m_raqueteProgram2 = createProgramFromFile(getAssetsPath() + "raquete.vert",
                                         getAssetsPath() + "raquete.frag");
                                         
  m_bolaProgram = createProgramFromFile(getAssetsPath() + "raquete.vert",
                                         getAssetsPath() + "raquete.frag");
  //Cor do background                                       
  glClearColor(0.83, 0.82, 0.82, 0.4);

#if !defined(__EMSCRIPTEN__)
  glEnable(GL_PROGRAM_POINT_SIZE);
#endif

  restart();
}

void OpenGLWindow::restart() {
  m_gameData.m_state = State::Playing;

  m_raquete.initializeGL(m_raqueteProgram);
  m_raquete2.initializeGL(m_raqueteProgram2);
  m_raquete2.m_translation = glm::vec2(0.8f, 0);
  directionX = -1;
  directionY = 0;

  m_bola.initializeGL(m_bolaProgram);
}

void OpenGLWindow::update() {

  float deltaTime{static_cast<float>(getDeltaTime())};

  if (m_gameData.m_state == State::AzulWin &&
      m_restartWaitTimer.elapsed() > 5) {
    restart();
    return;
  }
  else if (m_gameData.m_state == State::VermelhoWin &&
      m_restartWaitTimer.elapsed() > 5) {
    restart();
    return;
  }
  else if (m_gameData.m_state == State::Score &&
    m_restartWaitTimer.elapsed() > 2) {
    restart();
    return;
  }
  
  if (m_gameData.m_state == State::Playing) {
    checkCollisions();
    checkWinCondition();
  }

  if (m_gameData.m_input[static_cast<size_t>(Input::Up)] && m_raquete.m_translation.y < 0.8f){
    m_raquete.setTranslation(glm::vec2(m_raquete.m_translation.x, m_raquete.m_translation.y + (0.8 * deltaTime)));
  }
  if (m_gameData.m_input[static_cast<size_t>(Input::Down)] && m_raquete.m_translation.y > -0.8f){
    m_raquete.setTranslation(glm::vec2(m_raquete.m_translation.x, m_raquete.m_translation.y - (0.8 * deltaTime)));
  }
  if (m_gameData.m_input[static_cast<size_t>(Input::UpR)] && m_raquete2.m_translation.y < 0.8f){
    m_raquete2.setTranslation(glm::vec2(m_raquete2.m_translation.x, m_raquete2.m_translation.y + (0.8 * deltaTime)));
  }
  if (m_gameData.m_input[static_cast<size_t>(Input::DownR)] && m_raquete2.m_translation.y > -0.8f){
    m_raquete2.setTranslation(glm::vec2(m_raquete2.m_translation.x, m_raquete2.m_translation.y - (0.8 * deltaTime)));
  }

  m_bola.update(deltaTime, directionX, directionY);
  
}

void OpenGLWindow::paintGL() {
  update();

  glClear(GL_COLOR_BUFFER_BIT);
  glViewport(0, 0, m_viewportWidth, m_viewportHeight);

  m_raquete.paintGL(m_gameData);
  m_raquete2.paintGL(m_gameData);
  m_bola.paintGL(m_gameData);
}

void OpenGLWindow::paintUI() {
  abcg::OpenGLWindow::paintUI();

  {
    auto size{ImVec2(200, 200)};
    auto position{ImVec2((m_viewportWidth - size.x/2.0f) / 2.0f,
                         (m_viewportHeight - size.y/2.0f) / 2.0f)};
    ImGui::SetNextWindowPos(position);
    ImGui::SetNextWindowSize(size);
    ImGuiWindowFlags flags{ImGuiWindowFlags_NoBackground |
                           ImGuiWindowFlags_NoTitleBar |
                           ImGuiWindowFlags_NoInputs};
    ImGui::Begin(" ", nullptr, flags);
    ImGui::PushFont(m_font);

    if (m_gameData.m_state == State::Score) {
      ImGui::Text("%d - %d", player1Score, player2Score);
    } 
    if (m_gameData.m_state == State::AzulWin) {
       ImGui::Text("Vermelho");
       ImGui::Text("Ganhou!");
    }
    if (m_gameData.m_state == State::VermelhoWin) {
       ImGui::Text("Azul");
       ImGui::Text("Ganhou!");
    }

    ImGui::PopFont();
    ImGui::End();

  }
}

void OpenGLWindow::resizeGL(int width, int height) {
  m_viewportWidth = width;
  m_viewportHeight = height;

  glClear(GL_COLOR_BUFFER_BIT);
}

void OpenGLWindow::terminateGL() {
  glDeleteProgram(m_raqueteProgram);
  glDeleteProgram(m_raqueteProgram2);
  glDeleteProgram(m_bolaProgram);

  
  m_raquete.terminateGL();
  m_raquete2.terminateGL();
  m_bola.terminateGL();
}

void OpenGLWindow::checkCollisions() {
  freopen("logColisao.txt","w",stdout);

  glm::vec4 bolaAzul{0, 0, 1, 1};
  glm::vec4 bolaVermelha{1, 0, 0, 1};

  float distanciaRaquete1 = m_raquete.m_translation.x + (m_bola.m_translation.x)*-1;
  float distanciaRaquete2 = m_raquete2.m_translation.x - (m_bola.m_translation.x);

  //colisão raquete 1
  if(distanciaRaquete1 >= -0.04f){

    if(m_bola.m_color == bolaAzul && m_raquete.m_translation.y + 0.2f >= m_bola.m_translation.y && (m_raquete.m_translation.y - 0.2f < m_bola.m_translation.y)){
      if(m_raquete.m_translation.y == m_bola.m_translation.y){

        directionY = 0;
        directionX = directionX * -1;
      }
      else if(m_raquete.m_translation.y > m_bola.m_translation.y){

        directionY = -1;
        directionX = directionX * -1;
      }
      else if(m_raquete.m_translation.y < m_bola.m_translation.y){

        directionY = 1;
        directionX = directionX * -1;
      }
    }
  }
  //colisão raquete 2
  else if(distanciaRaquete2 <= 0.04f){

    if(m_bola.m_color == bolaVermelha && m_raquete2.m_translation.y + 0.2f >= m_bola.m_translation.y && (m_raquete2.m_translation.y - 0.2f <= m_bola.m_translation.y)){
      if(m_raquete2.m_translation.y == m_bola.m_translation.y){

        directionY = 0;
        directionX = directionX * -1;
      }
      else if(m_raquete2.m_translation.y > m_bola.m_translation.y){

        directionY = -1;
        directionX = directionX * -1;
      }
      else if(m_raquete2.m_translation.y < m_bola.m_translation.y){

        directionY = 1;
        directionX = directionX * -1;
      }
    }
  }

  //colisão com eixo y
  else if(m_bola.m_translation.y <= -1.0f){
    directionY = 1;
  }
  else if(m_bola.m_translation.y >= 1.0f){
    directionY = -1;
  }

}

void OpenGLWindow::checkWinCondition() {

  if(m_bola.m_translation.x >= 0.9f){
    player1Score = player1Score + 1;
    directionX = 0;
    if(player1Score == 5){
      m_gameData.m_state = State::AzulWin;
      player1Score = 0;
      player2Score = 0;
    }
    else{
      m_gameData.m_state = State::Score;
    }
    m_restartWaitTimer.restart();
  }
  else if(m_bola.m_translation.x <= -0.9f){
    player2Score = player2Score + 1;
    directionX = 0;
    if(player2Score == 5){
      m_gameData.m_state = State::VermelhoWin;
      player1Score = 0;
      player2Score = 0;
    }
    else{
      m_gameData.m_state = State::Score;
    }
    m_restartWaitTimer.restart();
  }

}