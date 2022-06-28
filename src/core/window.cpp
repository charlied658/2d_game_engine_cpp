//
// Created by Charlie Davidson on 6/27/22.
//

#include "core/window.h"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#define GL_SILENCE_DEPRECATION
#include <OpenGL/gl3.h>

#include "core/mouse_listener.h"
#include "core/key_listener.h"
#include "render/shader.h"
#include "render/render.h"
#include <cstdio>

static void error_callback(int error, const char* description) {
    printf("Error: %s\n", description);
}

namespace Window {

    static GLFWwindow* window;

    /**
     * Initializes the GLFW window
     * @return 0 on success, -1 on error
     */
    int init() {
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

        // Initialize OpenGL buffers
        Render::init();

        // Create and compile the shader program
        Shader::create_program();

        return 0;
    }

    /**
     * Enters the main loop for the GLFW window
     */
    void loop() {
        double startTime = glfwGetTime();
        double currentTime, dt;

        // Main loop
        while(!glfwWindowShouldClose(window)) {

            // Update dt (delta time)
            currentTime = glfwGetTime();
            dt = currentTime - startTime;
            startTime = currentTime;

            // Clear the screen
            glClear(GL_COLOR_BUFFER_BIT);

            // Set the shader program
            Shader::use_program();

            // Draw elements to the screen
            Render::draw(dt);

            // Swap buffers and poll events
            glfwSwapBuffers(window);
            glfwPollEvents();
        }

        // Terminate GLFW after the main loop has ended
        glfwTerminate();
    }
}