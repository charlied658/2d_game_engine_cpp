cmake_minimum_required(VERSION 3.22)
project(gameEngine)

set(CMAKE_CXX_STANDARD 14)

include_directories(gameEngine include)

file(GLOB SOURCES "src/*/*.cpp")
file(GLOB LIB "src/lib/*/*.cpp")
add_executable(gameEngine ${SOURCES} ${LIB} src/main.cpp)

#GLFW
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

add_subdirectory(glfw)
target_link_libraries(gameEngine glfw)

#GLAD
add_subdirectory(glad)
target_link_libraries(gameEngine glad)

#GLM
add_subdirectory(glm)
target_link_libraries(gameEngine glm)