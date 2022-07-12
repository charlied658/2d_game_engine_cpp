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
#include "render/sprite_renderer.h"
#include "render/line_renderer.h"
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
    static double fps;

    static unsigned int sprite_shader;
    static std::string sprite_vertex_filepath;
    static std::string sprite_fragment_filepath;

    static unsigned int line_shader;
    static std::string line_vertex_filepath;
    static std::string line_fragment_filepath;

    void window_size_callback(GLFWwindow* window_ptr, int w, int h)
    {
        width = w;
        height = h;

        // Adjust viewport
        int frame_width, frame_height;
        glfwGetFramebufferSize(window_ptr, &frame_width, &frame_height);
        glViewport(0, 0, frame_width, frame_height);
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

        // Load OpenGL with GLAD
        glfwMakeContextCurrent(window);
        if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
            printf("Failed to initialize OpenGL context");
            return -1;
        }
        glfwSwapInterval(1);

        // Maximize the window;
        glfwMaximizeWindow(window);

        // Sprite shader
        sprite_vertex_filepath = "assets/shaders/sprite/vertex.glsl";
        sprite_fragment_filepath = "assets/shaders/sprite/fragment.glsl";
        Shader::get_shader(&sprite_shader, sprite_vertex_filepath, sprite_fragment_filepath);

        // Line shader
        line_vertex_filepath = "assets/shaders/line/vertex.glsl";
        line_fragment_filepath = "assets/shaders/line/fragment.glsl";
        Shader::get_shader(&line_shader, line_vertex_filepath, line_fragment_filepath);


        // Initialize scene
        Scene::init();
        Camera::init();
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

            // Update the FPS (Frames per second)
            fps = 1.0f/dt;

            // Update the scene
            Scene::update(dt);

            // Clear the screen
            glClear(GL_COLOR_BUFFER_BIT);

            // Render lines
            Shader::use_program(line_shader);
            LineRenderer::render();

            // Draw sprites to the screen
            Shader::use_program(sprite_shader);
            SpriteRenderer::render();

            // Render ImGui elements
            ImGuiLayer::render();

            // Swap buffers and poll events
            glfwSwapBuffers(window);
            glfwPollEvents();
        }

        // Terminate GLFW after the main loop has ended
        glfwTerminate();
    }

    void close_window() {
        printf("Exiting...\n");
        glfwSetWindowShouldClose(window, true);
    }

    int get_width() {
        return width;
    }

    int get_height() {
        return height;
    }

    double get_fps() {
        return fps;
    }
}