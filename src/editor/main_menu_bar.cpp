/**
 * @author Charlie Davidson 
 * Created on 7/10/22.
 */

#include "editor/main_menu_bar.h"

#include "imgui/imgui.h"

#include "core/scene.h"
#include "core/window.h"
#include "editor/select_objects.h"

namespace MainMenuBar {

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
                    SelectObjects::copy_objects();
                }
                if (ImGui::MenuItem("Paste", "Ctrl+V")) {
                    SelectObjects::paste_objects();
                }
                if (ImGui::MenuItem("Delete")) {
                    SelectObjects::delete_objects();
                }
                ImGui::EndMenu();
            }
            ImGui::EndMainMenuBar();
        }
    }
}