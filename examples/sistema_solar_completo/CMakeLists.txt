project(sistema3d)
add_executable(${PROJECT_NAME} main.cpp model.cpp openglwindow.cpp camera.cpp  sistema.cpp)
enable_abcg(${PROJECT_NAME})
