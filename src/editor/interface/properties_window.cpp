/**
 * @author Charlie Davidson 
 * Created on 7/15/22.
 */

#include "properties_window.h"

#include "editor/scene.h"
#include "imgui/imgui.h"

namespace PropertiesWindow {

    /**
     * Display the properties window for the active game object.
     */
    void imgui() {

        Editor::GameObject::game_object **active_object;
        Editor::Scene::get_active_game_object(&active_object);
        Editor::GameObject::game_object *obj = *active_object;
        if (obj != nullptr) {
            ImGui::Begin("Properties");

            ImGui::Text("Game Object ID: %d", obj->id);
            ImGui::Text("Name: %s", obj->name.c_str());

            if (obj->has_sprite_manager) {
                if (ImGui::CollapsingHeader("Sprite Manager")) {
                    ImGui::Text("ID: %d", obj->spr_manager->id);
                    ImGui::Text("Position: %f,%f", obj->spr_manager->position.x, obj->spr_manager->position.y);
                    ImGui::Text("Grid position: %d,%d", obj->spr_manager->grid_x, obj->spr_manager->grid_y);
                    ImGui::Text("Scale: %f,%f", obj->spr_manager->scale.x, obj->spr_manager->scale.y);
                    ImGui::Text("Z-index: %d", obj->spr_manager->z_index);
                    ImGui::Text("Texture: %s", obj->spr_manager->sprite.texture_filepath.c_str());
                    ImGui::Text("Color: %f,%f,%f,%f", obj->spr_manager->color.x, obj->spr_manager->color.y,
                                obj->spr_manager->color.z, obj->spr_manager->color.w);
                    ImGui::Text("Saturation: %f", obj->spr_manager->saturation);
                }
            }
            if (obj->has_physics_manager) {
                if (ImGui::CollapsingHeader("Physics Manager")) {
                    ImGui::Text("ID: %d", obj->py_manager->id);
                }
            }
            if (obj->has_behavior_manager) {
                if (ImGui::CollapsingHeader("Behavior Manager")) {
                    ImGui::Text("ID: %d", obj->bh_manager->id);
                }
            }

            ImGui::End();
        }
    }
}
