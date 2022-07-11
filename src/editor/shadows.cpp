/**
 * @author Charlie Davidson 
 * Created on 7/10/22.
 */

#include "editor/shadows.h"

#include "render/render.h"
#include "editor/selected_objects.h"
#include "editor/holding_object.h"

namespace Shadows {

    static GameObject::game_object *shadow_objects;
    static int shadow_object_count;
    static GameObject::game_object holding_obj_shadow;

    static GameObject::game_object **selected_objects;
    static int selected_object_count;
    static GameObject::game_object *holding_object;
    bool holding_shadow_added;

    /**
     * Initialize shadow objects.
     */
    void init() {
        shadow_objects = new GameObject::game_object[1000];
        shadow_object_count = 0;
        holding_shadow_added = false;
    }

    /**
     * Reset variables when the scene is reloaded.
     */
    void reload() {
        shadow_object_count = 0;
    }

    /**
     * Render shadows under each selected game object.
     */
    void render() {
        // Update selected objects
        Selected::get_selected_objects(&selected_objects, &selected_object_count);

        // Render shadows for each selected game object
        for (int i = 0; i < selected_object_count; i++) {
            if (i < shadow_object_count) {
                GameObject::set_visible(&shadow_objects[i], true);
                GameObject::init(&shadow_objects[i], "shadow", selected_objects[i]->position + glm::vec2 {0.02f, -0.02f}, selected_objects[i]->scale, -5, &selected_objects[i]->sprite);
                GameObject::set_color(&shadow_objects[i], glm::vec4(0.0f, 0.0f, 0.0f, 0.7f));
                GameObject::set_pickable(&shadow_objects[i], false);
            } else {
                // Add new shadows if necessary
                shadow_objects[i] = GameObject::game_object {};
                GameObject::init(&shadow_objects[i], "shadow", selected_objects[i]->position + glm::vec2 {0.02f, -0.02f}, selected_objects[i]->scale, -5, &selected_objects[i]->sprite);
                GameObject::set_color(&shadow_objects[i], glm::vec4(0.0f, 0.0f, 0.0f, 0.7f));
                GameObject::set_pickable(&shadow_objects[i], false);
                Render::add_game_object(&shadow_objects[i]);
                shadow_object_count++;
            }
        }
        // Set the remaining shadows to be invisible
        if (shadow_object_count > selected_object_count) {
            for (int i = selected_object_count; i < shadow_object_count; i++) {
                GameObject::set_visible(&shadow_objects[i], false);
            }
        }

        // Put shadow under holding object
        Holding::get_holding_object(&holding_object);
        if (holding_object && holding_object->holding) {
            GameObject::set_visible(&holding_obj_shadow, true);
            GameObject::init(&holding_obj_shadow, "shadow", holding_object->position + glm::vec2 {0.02f, -0.02f}, holding_object->scale, -5, &holding_object->sprite);
            GameObject::set_color(&holding_obj_shadow, glm::vec4(0.0f, 0.0f, 0.0f, 0.7f));
            GameObject::set_pickable(&holding_obj_shadow, false);
            if (!holding_shadow_added) {
                holding_shadow_added = true;
                Render::add_game_object(&holding_obj_shadow);
            }
        } else {
            GameObject::set_visible(&holding_obj_shadow, false);
        }
    }
}
