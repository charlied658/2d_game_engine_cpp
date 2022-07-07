cmake_minimum_required(VERSION 3.22)
project(gameEngine)

set(CMAKE_CXX_STANDARD 14)

include_directories(gameEngine include)
include_directories(gameEngine include/lib)

file(GLOB SOURCES "src/*/*.cpp")
file(GLOB LIB "src/lib/*/*.cpp" "src/lib/*/*.c")
add_executable(gameEngine ${SOURCES} ${LIB} src/main.cpp)

# GLFW
add_subdirectory(lib/glfw)
include_directories(glfw/include)
target_link_libraries(gameEngine glfw ${GLFW_LIBRARIES})

# GLM
include_directories(lib/glm)