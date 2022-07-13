/**
 * @author Charlie Davidson 
 * Created on 7/12/22.
 */

#include "editor/preview_objects.h"

#include "core/game_object.h"
#include "render/sprite_renderer.h"
#include "editor/holding_object.h"
#include "editor/selected_objects.h"
#include "editor/drag_objects.h"
#include "util/collision_math.h"

namespace Preview {

    static GameObject::game_object *preview_objects;
    static int preview_object_count;
    static GameObject::game_object holding_object_preview;
    static bool holding_preview_added;

    static GameObject::game_object **selected_objects;
    static int selected_object_count;
    static GameObject::game_object *holding_object;

    /**
     * Initialize the preview objects.
     */
    void init() {
        preview_objects = new GameObject::game_object[1000];
        preview_object_count = 0;
        holding_preview_added = false;
    }

    /**
     * Reset the variables when the scene is reloaded.
     */
    void reload() {
        preview_object_count = 0;
        holding_preview_added = false;
    }

    /**
     * Update the preview objects.
     */
    void update() {
        // Render preview objects for each selected game object
        if (Drag::is_dragging_objects()) {
            Preview::render();
        } else {
            // If not dragging, set preview objects to be not visible
            Preview::hide();
        }

        // Render the holding object preview
        Preview::render_holding();
    }

    /**
     * Render preview objects.
     */
    void render() {
        Selected::get_selected_objects(&selected_objects, &selected_object_count);
        for (int i = 0; i < selected_object_count; i++) {
            glm::vec2 position = Math::grid_position(selected_objects[i]->position);
            generate_preview_object(&preview_objects[i], position,
                                    selected_objects[i]->scale, &selected_objects[i]->sprite);
            // Add new preview objects if necessary
            if (i >= preview_object_count && preview_object_count < 1000) {
                SpriteRenderer::add_game_object(&preview_objects[i]);
                preview_object_count++;
            }
        }
        // Set the remaining preview objects to be invisible
        if (preview_object_count > selected_object_count) {
            for (int i = selected_object_count; i < preview_object_count; i++) {
                GameObject::set_visible(&preview_objects[i], false);
            }
        }
    }

    /**
     * Render preview for holding object.
     */
    void render_holding() {
        Holding::get_holding_object(&holding_object);
        if (holding_object && Holding::is_holding()) {
            glm::vec2 position = Math::grid_position(holding_object->position);
            generate_preview_object(&holding_object_preview, position,
                                    holding_object->scale, &holding_object->sprite);
            if (!holding_preview_added) {
                holding_preview_added = true;
                SpriteRenderer::add_game_object(&holding_object_preview);
            }
        } else {
            GameObject::set_visible(&holding_object_preview, false);
        }
    }

    /**
     * Hide preview objects.
     */
    void hide() {
        for (int i = 0; i < preview_object_count; i++) {
            GameObject::set_visible(&preview_objects[i], false);
        }
    }

    void generate_preview_object(GameObject::game_object *obj, glm::vec2 position, glm::vec2 scale, Sprite::sprite *sprite) {
        GameObject::init(obj, "preview", position, scale, -5, sprite);
        GameObject::set_color(obj, glm::vec4(1.0f, 1.0f, 1.0f, 0.0f));
        GameObject::set_saturation(obj, 0.3f);
        obj->pickable = false;
    }
}
