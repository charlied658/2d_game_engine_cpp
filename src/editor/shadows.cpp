/**
 * @author Charlie Davidson 
 * Created on 7/10/22.
 */

#include "editor/shadows.h"

#include "render/sprite_renderer.h"
#include "editor/selected_objects.h"
#include "editor/holding_object.h"

namespace Shadows {

    static GameObject::game_object *shadow_objects;
    static int shadow_object_count;
    static GameObject::game_object holding_obj_shadow;

    static GameObject::game_object **selected_objects;
    static int selected_object_count;
    static GameObject::game_object *holding_object;
    static bool holding_shadow_added;

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
        holding_shadow_added = false;
    }

    /**
     * Render shadows under each selected game object.
     */
    void render() {
        // Update selected objects
        Selected::get_selected_objects(&selected_objects, &selected_object_count);

        // Render shadows for each selected game object
        for (int i = 0; i < selected_object_count; i++) {
            glm::vec2 position = selected_objects[i]->position + glm::vec2 {0.02f, -0.02f};
            generate_shadow(&shadow_objects[i], position, selected_objects[i]->scale, &selected_objects[i]->sprite);
            // Add new shadows if necessary
            if (i >= shadow_object_count && shadow_object_count < 1000) {
                SpriteRenderer::add_game_object(&shadow_objects[i]);
                shadow_object_count++;
            }
        }
        // Set the remaining shadows to be invisible
        if (shadow_object_count > selected_object_count) {
            for (int i = selected_object_count; i < shadow_object_count; i++) {
                GameObject::set_visible(&shadow_objects[i], false);
            }
        }

        Shadows::render_holding();
    }

    /**
     * Render shadow under holding object.
     */
    void render_holding() {
        // Put shadow under holding object
        Holding::get_holding_object(&holding_object);
        if (holding_object && Holding::is_holding()) {
            glm::vec2 position = holding_object->position + glm::vec2 {0.02f, -0.02f};
            generate_shadow(&holding_obj_shadow, position, holding_object->scale, &holding_object->sprite);
            if (!holding_shadow_added) {
                holding_shadow_added = true;
                SpriteRenderer::add_game_object(&holding_obj_shadow);
            }
        } else {
            GameObject::set_visible(&holding_obj_shadow, false);
        }
    }

    void generate_shadow(GameObject::game_object *obj, glm::vec2 position, glm::vec2 scale, Sprite::sprite *sprite) {
        GameObject::init(obj, "shadow", position, scale, 4, sprite);
        GameObject::set_color(obj, glm::vec4(0.0f, 0.0f, 0.0f, 0.7f));
        GameObject::set_saturation(obj, 1.0f);
        obj->pickable = false;
    }
}
