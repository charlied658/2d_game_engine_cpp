cmake_minimum_required(VERSION 3.22)
project(gameEngine)

set(CMAKE_CXX_STANDARD 14)

include_directories(gameEngine include)

add_executable(gameEngine src/main.cpp)

#GLFW
find_package(glfw3 3.3 REQUIRED)
target_link_libraries(gameEngine glfw)

#OpenGL
find_package(OpenGL REQUIRED)
target_link_libraries(gameEngine OpenGL::GL)
