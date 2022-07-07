/**
 * @author Charlie Davidson
 * Created on 6/27/22.
 */

#include "core/window.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "core/mouse_listener.h"
#include "core/key_listener.h"
#include "core/camera.h"
#include "core/scene.h"
#include "core/imgui_layer.h"
#include "render/shader.h"
#include "render/render.h"
#include <cstdio>

static void error_callback(int error, const char* description) {
    printf("Error: %s\n", description);
}

namespace Window {

    static GLFWwindow *window;
    static const int default_width = 1280;
    static const int default_height = 720;
    static const char *title = "Game Engine";
    static int width, height;

    void window_size_callback(GLFWwindow* window_ptr, int w, int h)
    {
        width = w;
        height = h;
    }

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
        window = glfwCreateWindow(default_width, default_height, title, nullptr, nullptr);
        if (!window) {
            glfwTerminate();
            return -1;
        }

        // Set window size callback
        glfwSetWindowSizeCallback(window, window_size_callback);

        // Set mouse and key callbacks
        glfwSetCursorPosCallback(window, Mouse::cursor_position_callback);
        glfwSetMouseButtonCallback(window, Mouse::mouse_button_callback);
        glfwSetScrollCallback(window, Mouse::scroll_callback);
        glfwSetKeyCallback(window, Key::key_callback);

        // Maximize the window;
        glfwMaximizeWindow(window);

        // Load OpenGL with GLAD
        glfwMakeContextCurrent(window);
        if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
            printf("Failed to initialize OpenGL context");
            return -1;
        }
        glfwSwapInterval(1);

        // Initialize scene
        Scene::init();
        Camera::init();
        Shader::create_program();
        ImGuiLayer::init(window);

        return 0;
    }

    /**
     * Enters the main loop for the GLFW window
     */
    void loop() {
        double startTime = glfwGetTime();
        double currentTime, dt;

        // Set the clear color
        glClearColor(0.9f,0.9f,0.9f,1.0f);

        // Main loop
        while(!glfwWindowShouldClose(window)) {

            // Update dt (delta time)
            currentTime = glfwGetTime();
            dt = currentTime - startTime;
            startTime = currentTime;

            // Print the FPS (Frames per second)
            //printf("FPS: %f\n", 1/dt);

            // Update the scene
            Scene::update(dt);

            // Clear the screen
            glClear(GL_COLOR_BUFFER_BIT);

            // Draw sprites to the screen
            Shader::use_program();
            Render::render();

            // Render ImGui elements
            ImGuiLayer::render();

            // Swap buffers and poll events
            glfwSwapBuffers(window);
            glfwPollEvents();
        }

        // Terminate GLFW after the main loop has ended
        glfwTerminate();
    }

    int get_width() {
        return width;
    }

    int get_height() {
        return height;
    }
}