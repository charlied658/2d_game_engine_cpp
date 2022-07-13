/**
 * @author Charlie Davidson 
 * Created on 7/12/22.
 */

#include "editor/preview_objects.h"

#include "core/game_object.h"
#include "render/sprite_renderer.h"
#include "editor/holding_object.h"
#include "editor/selected_objects.h"
#include "editor/mouse_events.h"
#include "util/collision_math.h"

namespace Preview {

    static GameObject::game_object *preview_objects;
    static int preview_object_count;
    static GameObject::game_object holding_object_preview;
    static bool holding_preview_added;

    static GameObject::game_object **selected_objects;
    static int selected_object_count;
    static GameObject::game_object *holding_object;

    static const float block_width = 0.25f;
    static const float block_height = 0.25f;

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
     * Render the preview objects.
     */
    void render() {

        // Update selected objects
        Selected::get_selected_objects(&selected_objects, &selected_object_count);

        // Render shadows for each selected game object
        if (Mouse::is_dragging_objects()) {
            for (int i = 0; i < selected_object_count; i++) {
                if (i < preview_object_count) {
                    GameObject::set_visible(&preview_objects[i], true);
                    glm::vec2 centered_position = {selected_objects[i]->position.x + block_width / 2,
                                                   selected_objects[i]->position.y + block_height / 2};
                    glm::vec2 position = {centered_position.x - Math::f_mod(centered_position.x, block_width),
                                          centered_position.y - Math::f_mod(centered_position.y, block_height)};
                    GameObject::init(&preview_objects[i], "preview", position, selected_objects[i]->scale, -5,
                                     &selected_objects[i]->sprite);
                    GameObject::set_color(&preview_objects[i], glm::vec4(1.0f, 1.0f, 1.0f, 0.0f));
                    GameObject::set_saturation(&preview_objects[i], 0.3f);
                    preview_objects[i].pickable = false;
                } else {
                    // Add new shadows if necessary
                    if (preview_object_count < 1000) {
                        preview_objects[i] = GameObject::game_object{};
                        glm::vec2 centered_position = {selected_objects[i]->position.x + block_width / 2,
                                                       selected_objects[i]->position.y + block_height / 2};
                        glm::vec2 position = {centered_position.x - Math::f_mod(centered_position.x, block_width),
                                              centered_position.y - Math::f_mod(centered_position.y, block_height)};
                        GameObject::init(&preview_objects[i], "preview", position, selected_objects[i]->scale, -5,
                                         &selected_objects[i]->sprite);
                        GameObject::set_color(&preview_objects[i], glm::vec4(1.0f, 1.0f, 1.0f, 0.0f));
                        GameObject::set_saturation(&preview_objects[i], 0.3f);
                        preview_objects[i].pickable = false;
                        SpriteRenderer::add_game_object(&preview_objects[i]);
                        preview_object_count++;
                    }
                }
            }
            // Set the remaining shadows to be invisible
            if (preview_object_count > selected_object_count) {
                for (int i = selected_object_count; i < preview_object_count; i++) {
                    GameObject::set_visible(&preview_objects[i], false);
                }
            }
        } else {
            for (int i = 0; i < preview_object_count; i++) {
                GameObject::set_visible(&preview_objects[i], false);
            }
        }

        // Render the holding object preview
        Holding::get_holding_object(&holding_object);
        if (holding_object && Holding::is_holding()) {
            GameObject::set_visible(&holding_object_preview, true);
            glm::vec2 centered_position = {holding_object->position.x + block_width / 2, holding_object->position.y + block_height / 2};
            glm::vec2 position = {centered_position.x - Math::f_mod(centered_position.x, block_width), centered_position.y - Math::f_mod(centered_position.y, block_height)};
            GameObject::init(&holding_object_preview, "preview", position, holding_object->scale, -5, &holding_object->sprite);
            GameObject::set_color(&holding_object_preview, glm::vec4(1.0f, 1.0f, 1.0f, 0.0f));
            GameObject::set_saturation(&holding_object_preview, 0.3f);
            holding_object_preview.pickable = false;
            if (!holding_preview_added) {
                holding_preview_added = true;
                SpriteRenderer::add_game_object(&holding_object_preview);
            }
        } else {
            GameObject::set_visible(&holding_object_preview, false);
        }
    }
}
