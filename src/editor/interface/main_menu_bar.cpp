/**
 * @author Charlie Davidson 
 * Created on 7/10/22.
 */

#include "main_menu_bar.h"

#include "imgui/imgui.h"

#include "editor/scene.h"
#include "core/window.h"
#include "copied_objects.h"

namespace MainMenuBar {

    static bool info_window;
    static bool object_picker;
    static bool imgui_demo_window;
    static bool properties_window;

    /**
     * Initialize main menu bar.
     */
    void init() {
        info_window = false;
        object_picker = true;
        imgui_demo_window = false;
        properties_window = true;
    }

    /**
     * Render the main menu bar along the top of the screen.
     */
    void imgui() {
        if (ImGui::BeginMainMenuBar()) {
            // Level loading / saving system
            if (ImGui::BeginMenu("File")) {
                if (ImGui::MenuItem("New", "Ctrl+N")) {
                    Editor::Scene::new_level();
                }
                if (ImGui::MenuItem("Load", "Ctrl+L")) {
                    Editor::Scene::load_level();
                }
                if (ImGui::MenuItem("Save", "Ctrl+S")) {
                    Editor::Scene::save_level();
                }
                if (ImGui::MenuItem("Save & Exit")) {
                    Editor::Scene::save_level();
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
                if (ImGui::MenuItem("Properties", nullptr, properties_window)) {
                    properties_window = !properties_window;
                }
                if (ImGui::MenuItem("ImGui Demo", nullptr, imgui_demo_window)) {
                    imgui_demo_window = !imgui_demo_window;
                }
                ImGui::EndMenu();
            }
            ImGui::EndMainMenuBar();
        }
    }

    bool show_info_window() {
        return info_window;
    }

    bool show_object_picker() {
        return object_picker;
    }

    bool show_imgui_demo_window() {
        return imgui_demo_window;
    }

    bool show_properties_window() {
        return properties_window;
    }
}