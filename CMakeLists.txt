cmake_minimum_required(VERSION 3.22)
project(gameEngine)

set(CMAKE_CXX_STANDARD 14)

include_directories(gameEngine include)

add_executable(gameEngine src/main.cpp)

#OpenGL
find_package(OpenGL REQUIRED)
target_link_libraries(gameEngine OpenGL::GL)

#GLUT
find_package(GLUT REQUIRED)
target_link_libraries(gameEngine GLUT::GLUT)