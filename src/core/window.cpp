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
#include "render/camera.h"
#include "render/shader.h"
#include "render/render.h"
#include <cstdio>

static void error_callback(int error, const char* description) {
    printf("Error: %s\n", description);
}

namespace Window {

    static GLFWwindow* window;
    static const int default_width = 640;
    static const int default_height = 480;
    static const char *title = "Game Engine";
    static float aspect_ratio = (float) default_width / (float) default_height;

    void window_size_callback(GLFWwindow* window_ptr, int w, int h)
    {
        aspect_ratio = (float) w / (float) h;
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

        // Make OpenGL context current
        glfwMakeContextCurrent(window);

        // Initialize OpenGL buffers
        Render::init();

        // Initialize the camera
        Camera::init();

        // Create and compile the shader program
        Shader::create_program();

        glEnable(GL_DEPTH_TEST);

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
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            // Set the shader program
            Shader::use_program();

            // ================ Camera controls
            // Move camera
            if (Key::get_key_pressed(GLFW_KEY_LEFT) && !Key::get_key_pressed(GLFW_KEY_RIGHT)) {
                Camera::move_camera(glm::vec2 (dt, 0));
            }
            if (Key::get_key_pressed(GLFW_KEY_RIGHT) && !Key::get_key_pressed(GLFW_KEY_LEFT)) {
                Camera::move_camera(glm::vec2 (-dt, 0));
            }
            if (Key::get_key_pressed(GLFW_KEY_UP) && !Key::get_key_pressed(GLFW_KEY_DOWN)) {
                Camera::move_camera(glm::vec2 (0, -dt));
            }
            if (Key::get_key_pressed(GLFW_KEY_DOWN) && !Key::get_key_pressed(GLFW_KEY_UP)) {
                Camera::move_camera(glm::vec2 (0, dt));
            }

            // Zoom in/out
//            if (Key::get_key_pressed(GLFW_KEY_W) && !Key::get_key_pressed(GLFW_KEY_S)) {
//                Camera::scale_camera(1.01f);
//            }
//            if (Key::get_key_pressed(GLFW_KEY_S) && !Key::get_key_pressed(GLFW_KEY_W)) {
//                Camera::scale_camera(1/1.01f);
//            }

            // Rotate
            if (Key::get_key_pressed(GLFW_KEY_A) && !Key::get_key_pressed(GLFW_KEY_D)) {
                Camera::rotate_camera(glm::vec2 (dt, 0));
            }
            if (Key::get_key_pressed(GLFW_KEY_D) && !Key::get_key_pressed(GLFW_KEY_A)) {
                Camera::rotate_camera(glm::vec2 (-dt, 0));
            }
            if (Key::get_key_pressed(GLFW_KEY_W) && !Key::get_key_pressed(GLFW_KEY_S)) {
                Camera::rotate_camera(glm::vec2 (0, -dt));
            }
            if (Key::get_key_pressed(GLFW_KEY_S) && !Key::get_key_pressed(GLFW_KEY_W)) {
                Camera::rotate_camera(glm::vec2 (0, dt));
            }

            // Reset camera
            if (Key::get_key_begin_press(GLFW_KEY_SPACE)) {
                Camera::init();
            }
            // =============================

            // Draw elements to the screen
            Render::draw(dt);

            // Swap buffers and poll events
            glfwSwapBuffers(window);
            glfwPollEvents();
        }

        // Terminate GLFW after the main loop has ended
        glfwTerminate();
    }

    float get_aspect_ratio() {
        return aspect_ratio;
    }
}