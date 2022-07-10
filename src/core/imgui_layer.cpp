/**
 * @author Charlie Davidson 
 * Created on 7/7/22.
 */

#include "core/imgui_layer.h"

#include "core/scene.h"
#include "editor/imgui/info_window.h"

namespace ImGuiLayer {

    /**
     * Initialize ImGui context.
     * @param window GLFW window pointer
     */
    void init(GLFWwindow *window) {
        ImGui::CreateContext();
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init(nullptr);
    }

    /**
     * Render ImGui data to the screen.
     */
    void render() {
        // Create a new frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // All ImGui draw functions go here
        if (InfoWindow::show_demo_window()) {
            ImGui::ShowDemoWindow();
        }
        Scene::imgui();

        // Render data
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

    bool want_mouse_capture() {
        ImGuiIO io = ImGui::GetIO();
        return io.WantCaptureMouse;
    }
}
