/**
 * @author Charlie Davidson 
 * Created on 7/10/22.
 */

#include "editor/imgui/main_menu_bar.h"

#include "imgui/imgui.h"

#include "core/scene.h"
#include "core/window.h"
#include "editor/copied_objects.h"

namespace MainMenuBar {

    static bool info_window;
    static bool object_picker;
    static bool imgui_demo_window;

    /**
     * Initialize main menu bar.
     */
    void init() {
        info_window = false;
        object_picker = true;
        imgui_demo_window = false;
    }

    /**
     * Render the main menu bar along the top of the screen.
     */
    void imgui() {
        if (ImGui::BeginMainMenuBar()) {
            // Level loading / saving system
            if (ImGui::BeginMenu("File")) {
                if (ImGui::MenuItem("New", "Ctrl+N")) {
                    Scene::new_level();
                }
                if (ImGui::MenuItem("Load", "Ctrl+L")) {
                    Scene::load_level();
                }
                if (ImGui::MenuItem("Save", "Ctrl+S")) {
                    Scene::save_level();
                }
                if (ImGui::MenuItem("Save & Exit")) {
                    Scene::save_level();
                    Window::close_window();
                }
                if (ImGui::MenuItem("Exit")) {
                    Window::close_window();
                }
                ImGui::EndMenu();
            }
            // Copy / paste / delete objects
            if (ImGui::BeginMenu("Edit")) {
                if (ImGui::MenuItem("Copy", "Ctrl+C")) {
                    Copy::copy_objects();
                }
                if (ImGui::MenuItem("Paste", "Ctrl+V")) {
                    Copy::paste_objects();
                }
                if (ImGui::MenuItem("Delete")) {
                    Copy::delete_objects();
                }
                ImGui::EndMenu();
            }
            // View windows
            if (ImGui::BeginMenu("View")) {
                if (ImGui::MenuItem("Info", nullptr, info_window)) {
                    info_window = !info_window;
                }
                if (ImGui::MenuItem("Object Picker", nullptr, object_picker)) {
                    object_picker = !object_picker;
                }
                if (ImGui::MenuItem("ImGui Demo", nullptr, imgui_demo_window)) {
                    imgui_demo_window = !imgui_demo_window;
                }
                ImGui::EndMenu();
            }
            ImGui::EndMainMenuBar();
        }
    }

    /**
     * Get whether the info window should show.
     * @return Show info window
     */
    bool show_info_window() {
        return info_window;
    }

    /**
     * Get whether the object picker should show.
     * @return Show object picker
     */
    bool show_object_picker() {
        return object_picker;
    }

    /**
     * Get whether the ImGui demo window should show.
     * @return Show ImGui demo
     */
    bool show_imgui_demo_window() {
        return imgui_demo_window;
    }
}