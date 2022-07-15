/**
 * @author Charlie Davidson 
 * Created on 7/10/22.
 */

#include "shadows.h"

#include "editor/render/sprite_renderer.h"
#include "selected_objects.h"
#include "holding_object.h"
#include "editor/sprite_system.h"

namespace Shadows {

    static Editor::GameObject::game_object *shadow_objects;
    static int shadow_object_count;
    static Editor::GameObject::game_object holding_obj_shadow;

    static Editor::GameObject::game_object **selected_objects;
    static int selected_object_count;
    static Editor::GameObject::game_object *holding_object;
    static bool holding_shadow_added;

    /**
     * Initialize shadow objects.
     */
    void init() {
        shadow_objects = new Editor::GameObject::game_object[1000];
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
            if (i >= shadow_object_count && shadow_object_count < 1000) {
                Editor::SpriteSystem::init_sprite_manager(&shadow_objects[i].spr_manager);
            }
            glm::vec2 position = selected_objects[i]->spr_manager->position + glm::vec2 {0.02f, -0.02f};
            generate_shadow(shadow_objects[i].spr_manager, position, selected_objects[i]->spr_manager->scale, &selected_objects[i]->spr_manager->sprite);
            // Add new shadows if necessary
            if (i >= shadow_object_count && shadow_object_count < 1000) {
                Editor::SpriteRenderer::add_sprite(shadow_objects[i].spr_manager);
                shadow_object_count++;
            }
        }
        // Set the remaining shadows to be invisible
        if (shadow_object_count > selected_object_count) {
            for (int i = selected_object_count; i < shadow_object_count; i++) {
                Editor::SpriteManager::set_visible(shadow_objects[i].spr_manager, false);
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
        if (!holding_shadow_added) {
            Editor::SpriteSystem::init_sprite_manager(&holding_obj_shadow.spr_manager);
            holding_shadow_added = true;
            Editor::SpriteRenderer::add_sprite(holding_obj_shadow.spr_manager);
        }
        if (holding_object && Holding::is_holding()) {
            glm::vec2 position = holding_object->spr_manager->position + glm::vec2 {0.02f, -0.02f};
            generate_shadow(holding_obj_shadow.spr_manager, position, holding_object->spr_manager->scale, &holding_object->spr_manager->sprite);
        } else {
            Editor::SpriteManager::set_visible(holding_obj_shadow.spr_manager, false);
        }
    }

    void generate_shadow(Editor::SpriteManager::sprite_manager *obj, glm::vec2 position, glm::vec2 scale, Sprite::sprite *sprite) {
        Editor::SpriteManager::init(obj, position, scale, 4, sprite);
        Editor::SpriteManager::set_color(obj, glm::vec4(0.0f, 0.0f, 0.0f, 0.7f));
        Editor::SpriteManager::set_saturation(obj, 1.0f);
        obj->pickable = false;
    }
}
