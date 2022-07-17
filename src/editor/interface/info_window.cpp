/**
 * @author Charlie Davidson 
 * Created on 7/10/22.
 */

#include "info_window.h"

#include "imgui/imgui.h"

#include "core/window.h"
#include "core/camera.h"
#include "object_manager.h"
#include "editor/scene.h"
#include "editor/game_object.h"
#include "editor/sprite_system.h"
#include "editor/physics_system.h"
#include "editor/behavior_system.h"
#include "editor/render/sprite_renderer.h"

namespace InfoWindow {

    static Editor::GameObject::game_object *game_objects_list;
    static int game_objects_count;
    static Editor::GameObject::game_object *transient_game_objects_list;
    static int transient_game_objects_count;
    static Editor::SpriteManager::sprite_manager *sprite_manager_list;
    static int sprite_manager_count;
    static Editor::PhysicsManager::physics_manager *physics_manager_list;
    static int physics_manager_count;
    static Editor::BehaviorManager::behavior_manager *behavior_manager_list;
    static int behavior_manager_count;

    /**
     * Render the info window.
     */
    void imgui() {
        Editor::Scene::get_game_objects_list(&game_objects_list,&game_objects_count);
        Editor::Scene::get_transient_game_objects_list(&transient_game_objects_list, &transient_game_objects_count);
        Editor::SpriteSystem::get_sprite_manager_list(&sprite_manager_list,&sprite_manager_count);
        Editor::PhysicsSystem::get_physics_manager_list(&physics_manager_list,&physics_manager_count);
        Editor::BehaviorSystem::get_behavior_manager_list(&behavior_manager_list,&behavior_manager_count);

        ImGui::Begin("Info");
        if (ImGui::BeginTabBar("TabBar"))
        {
            if (ImGui::BeginTabItem("Selected")) {
                ObjectManager::imgui();
                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem("Camera")) {
                ImGui::Text("FPS: %f", Window::get_fps());
                ImGui::Text("Zoom: %f", Camera::getZoom());
                ImGui::Text("Camera position: %f, %f", Camera::get_position().x, Camera::get_position().y);
                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem("Objects")) {
                ImGui::Text("Game objects: %d", game_objects_count);
                ImGui::Text("Transient game objects: %d", transient_game_objects_count);
                ImGui::Text("Sprite managers: %d", sprite_manager_count);
                ImGui::Text("Physics managers: %d", physics_manager_count);
                ImGui::Text("Behavior managers: %d", behavior_manager_count);
                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem("Renderer")) {
                Editor::SpriteRenderer::imgui();
                ImGui::EndTabItem();
            }
            ImGui::EndTabBar();
        }
        ImGui::End();
    }
}
