/**
 * @author Charlie Davidson
 */

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#define GL_SILENCE_DEPRECATION
#include <OpenGL/gl3.h>

#include "mouse_listener.h"
#include "key_listener.h"
#include "vecmath.h"
#include <cstdio>

const char* vertex_shader_text =
        "#version 330 core\n"
        "layout (location = 0) in vec3 aPos;\n"
        "layout (location = 1) in vec3 aColor;\n"
        "out vec3 fColor;"
        "void main() {\n"
        "    fColor = aColor;\n"
        "    gl_Position = vec4(aPos, 1.0);\n"
        "}";

const char* fragment_shader_text =
        "#version 330 core\n"
        "in vec3 fColor;\n"
        "out vec4 color;\n"
        "void main() {\n"
        "    color = vec4(fColor, 1.0f);\n"
        "}";

struct vertex {
        float x, y, z;
        float r, g, b;
};

vertex vertices[] = {
        {-0.5f, -0.5f, 0.0f, 1.0f,0.0f,0.0f},
        {0.5f, -0.5f, 0.0f, 0.0f,1.0f,0.0f},
        {0.5f,  0.5f, 0.0f, 0.0f,0.0f,1.0f},
        {-0.5f,  0.5f, 0.0f, 0.0f,1.0f,1.0f}
};

/*
 * ========= Diagram of one Quad: ==========
 *
 *               3-----------2
 *               |           |
 *               |           |
 *               |           |
 *               0-----------1
 *
 *  Triangles:
 *      0 -> 1 -> 2
 *      0 -> 2 -> 3
 *
 *  NOTE: Vertices should be given in COUNTER-CLOCKWISE
 *  order according to OpenGL
 *
 */

int element_indices[] = {0,1,2, 0,2,3};

static void error_callback(int error, const char* description) {
    printf("Error: %s\n", description);
}

int main()
{
    GLFWwindow* window;
    GLuint vertex_shader, fragment_shader, program;
    GLuint vaoID, vboID, eboID;
    GLint success, len;
    GLsizei length;
    GLchar *description;

    // Set error callback
    glfwSetErrorCallback(error_callback);

    // Initialize GLFW
    if (!glfwInit()) {
        return -1;
    }

    // Set GLFW window hints
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create GLFW window
    window = glfwCreateWindow(640, 480, "My Window", nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    // Set mouse and key callbacks
    glfwSetCursorPosCallback(window, Mouse::cursor_position_callback);
    glfwSetMouseButtonCallback(window, Mouse::mouse_button_callback);
    glfwSetScrollCallback(window, Mouse::scroll_callback);
    glfwSetKeyCallback(window, Key::key_callback);

    // Make OpenGL context current
    glfwMakeContextCurrent(window);

    // Generate Vertex Array Object (VAO)
    glGenVertexArrays(1, &vaoID);
    glBindVertexArray(vaoID);

    // Generate Vertex Buffer Object (VBO)
    glGenBuffers(1, &vboID);
    glBindBuffer(GL_ARRAY_BUFFER, vboID);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);

    // Generate Element Buffer Object (EBO)
    glGenBuffers(1, &eboID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(element_indices), element_indices, GL_STATIC_DRAW);

    // Enable vertex attributes
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6,nullptr);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (void*)(sizeof(float) * 3));
    glEnableVertexAttribArray(1);

    // Compile vertex shader
    vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_shader_text, nullptr);
    glCompileShader(vertex_shader);
    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
    if (success == GL_FALSE) {
        glGetShaderiv(vertex_shader, GL_INFO_LOG_LENGTH, &len);
        glGetShaderInfoLog(vertex_shader, len, &length, description);
        printf("Error: Vertex shader compilation failed\n");
        printf("%s", description);
    }

    // Compile fragment shader
    fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragment_shader_text, nullptr);
    glCompileShader(fragment_shader);
    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
    if (success == GL_FALSE) {
        glGetShaderiv(fragment_shader, GL_INFO_LOG_LENGTH, &len);
        glGetShaderInfoLog(fragment_shader, len, &length, description);
        printf("Error: Fragment shader compilation failed\n");
        printf("%s", description);
    }

    // Link shaders
    program = glCreateProgram();
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    glLinkProgram(program);
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (success == GL_FALSE) {
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &len);
        glGetProgramInfoLog(program, len, &length, description);
        printf("Error: Linking of shaders failed\n");
        printf("%s", description);
    }

    double startTime = glfwGetTime();
    double currentTime, dt;

    // Main loop
    while(!glfwWindowShouldClose(window)) {

        // Update dt (delta time)
        currentTime = glfwGetTime();
        dt = currentTime - startTime;
        startTime = currentTime;

        // User input
        if (Key::get_key_pressed(GLFW_KEY_LEFT)) {
            for (int i = 0; i < 4; i++) {
                vector2 point = {vertices[i].x, vertices[i].y};
                vector2 result = Math::rotate(point, vector2{0,0}, dt * 0.5f);
                vertices[i].x = result.x;
                vertices[i].y = result.y;
            }
        } else if (Key::get_key_pressed(GLFW_KEY_RIGHT)) {
            for (int i = 0; i < 4; i++) {
                vector2 point = {vertices[i].x, vertices[i].y};
                vector2 result = Math::rotate(point, vector2{0,0}, dt * -0.5f);
                vertices[i].x = result.x;
                vertices[i].y = result.y;
            }
        }

        // Clear the screen
        glClear(GL_COLOR_BUFFER_BIT);

        // Update the buffer data
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

        // Draw elements to the screen
        glUseProgram(program);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

        // Swap buffers and poll events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Terminate GLFW after the main loop has ended
    glfwTerminate();
    return 0;
}