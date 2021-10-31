project(pong)

add_executable(${PROJECT_NAME} main.cpp openglwindow.cpp raquete.cpp bola.cpp raquete_player2.cpp)

enable_abcg(${PROJECT_NAME})
