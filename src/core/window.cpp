/**
 * @author Charlie Davidson
 * Created on 6/27/22.
 */

#include "core/window.h"

#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include "lib/imgui/imgui.h"
#include "lib/imgui/imgui_impl_glfw.h"
#include "lib/imgui/imgui_impl_opengl3.h"

#include "core/mouse_listener.h"
#include "core/key_listener.h"
#include "core/camera.h"
#include "core/scene.h"
#include "render/shader.h"
#include "render/render.h"
#include <cstdio>

static void error_callback(int error, const char* description) {
    printf("Error: %s\n", description);
}

namespace Window {

    static GLFWwindow* window;
    static const int default_width = 1280;
    static const int default_height = 720;
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

        // Maximize the window;
        glfwMaximizeWindow(window);

        // Initialize the scene
        Scene::init();

        // Initialize the camera
        Camera::init();

        // Create and compile the shader program
        Shader::create_program();

        // Create ImGui context
        ImGui::CreateContext();

        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init(nullptr);

        return 0;
    }

    /**
     * Enters the main loop for the GLFW window
     */
    void loop() {
        double startTime = glfwGetTime();
        double currentTime, dt;

        glClearColor(0.9f,0.9f,0.9f,1.0f);

        // Main loop
        while(!glfwWindowShouldClose(window)) {

            // Setup ImGui
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            // Show ImGui Demo Window
            ImGui::ShowDemoWindow();

            ImGui::Render();

            // Update dt (delta time)
            currentTime = glfwGetTime();
            dt = currentTime - startTime;
            startTime = currentTime;

            // Clear the screen
            glClear(GL_COLOR_BUFFER_BIT);

            // Set the shader program
            Shader::use_program();

            // Update the scene
            Scene::update(dt);

            // ================ Camera controls
            // Move camera
            if (Key::get_key_pressed(GLFW_KEY_LEFT) && !Key::get_key_pressed(GLFW_KEY_RIGHT)) {
                Camera::move_camera(glm::vec2 (dt * 1.5, 0));
            }
            if (Key::get_key_pressed(GLFW_KEY_RIGHT) && !Key::get_key_pressed(GLFW_KEY_LEFT)) {
                Camera::move_camera(glm::vec2 (-dt * 1.5, 0));
            }
            if (Key::get_key_pressed(GLFW_KEY_UP) && !Key::get_key_pressed(GLFW_KEY_DOWN)) {
                Camera::move_camera(glm::vec2 (0, -dt * 1.5));
            }
            if (Key::get_key_pressed(GLFW_KEY_DOWN) && !Key::get_key_pressed(GLFW_KEY_UP)) {
                Camera::move_camera(glm::vec2 (0, dt * 1.5));
            }

            // Zoom in/out
            if (Key::get_key_pressed(GLFW_KEY_W) && !Key::get_key_pressed(GLFW_KEY_S)) {
                Camera::scale_camera(1.01f);
            }
            if (Key::get_key_pressed(GLFW_KEY_S) && !Key::get_key_pressed(GLFW_KEY_W)) {
                Camera::scale_camera(1/1.01f);
            }

            // Reset camera
            if (Key::get_key_begin_press(GLFW_KEY_SPACE)) {
                Camera::init();
            }
            // =============================

            // Draw elements to the screen
            Render::render();

            // Render ImGui elements
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

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