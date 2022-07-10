/**
 * @author Charlie Davidson 
 * Created on 7/10/22.
 */

#include "editor/shadows.h"

#include "render/render.h"
#include "editor/selected_objects.h"

namespace Shadows {

    static GameObject::game_object *shadow_objects;
    static int shadow_object_count;

    static GameObject::game_object **selected_objects;
    static int selected_object_count;

    /**
     * Initialize shadow objects.
     */
    void init() {
        shadow_objects = new GameObject::game_object[1000];
        shadow_object_count = 0;
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
    }
}
