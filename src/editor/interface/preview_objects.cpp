/**
 * @author Charlie Davidson 
 * Created on 7/12/22.
 */

#include "preview_objects.h"

#include "editor/sprite_manager.h"
#include "editor/render/sprite_renderer.h"
#include "holding_object.h"
#include "selected_objects.h"
#include "drag_objects.h"
#include "editor/collision/collision_math.h"
#include "editor/sprite_system.h"

namespace Preview {

    static Editor::GameObject::game_object *preview_objects;
    static int preview_object_count;
    static Editor::GameObject::game_object holding_object_preview;
    static bool holding_preview_added;

    static Editor::GameObject::game_object **selected_objects;
    static int selected_object_count;
    static Editor::GameObject::game_object *holding_object;

    /**
     * Initialize the preview objects.
     */
    void init() {
        preview_objects = new Editor::GameObject::game_object[1000];
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
            if (i >= preview_object_count && preview_object_count < 1000) {
                Editor::SpriteSystem::add_sprite_manager(&preview_objects[i].spr_manager);
            }
            glm::vec2 position = Math::grid_position(selected_objects[i]->spr_manager->position);
            generate_preview_object(preview_objects[i].spr_manager, position,
                                    selected_objects[i]->spr_manager->scale, &selected_objects[i]->spr_manager->sprite);
            // Add new preview objects if necessary
            if (i >= preview_object_count && preview_object_count < 1000) {
                Editor::SpriteRenderer::add_sprite(preview_objects[i].spr_manager);
                preview_object_count++;
            }
        }
        // Set the remaining preview objects to be invisible
        if (preview_object_count > selected_object_count) {
            for (int i = selected_object_count; i < preview_object_count; i++) {
                Editor::SpriteManager::set_visible(preview_objects[i].spr_manager, false);
            }
        }
    }

    /**
     * Render preview for holding object.
     */
    void render_holding() {
        Holding::get_holding_object(&holding_object);
        if (holding_object && Holding::is_holding()) {
            if (!holding_preview_added) {
                Editor::SpriteSystem::add_sprite_manager(&holding_object_preview.spr_manager);
            }
            glm::vec2 position = Math::grid_position(holding_object->spr_manager->position);
            generate_preview_object(holding_object_preview.spr_manager, position,
                                    holding_object->spr_manager->scale, &holding_object->spr_manager->sprite);
            if (!holding_preview_added) {
                holding_preview_added = true;
                Editor::SpriteRenderer::add_sprite(holding_object_preview.spr_manager);
            }
        } else {
            if (holding_preview_added) {
                Editor::SpriteManager::set_visible(holding_object_preview.spr_manager, false);
            }
        }
    }

    /**
     * Hide preview objects.
     */
    void hide() {
        for (int i = 0; i < preview_object_count; i++) {
            Editor::SpriteManager::set_visible(preview_objects[i].spr_manager, false);
        }
    }

    void generate_preview_object(Editor::SpriteManager::sprite_manager *obj, glm::vec2 position, glm::vec2 scale, Sprite::sprite *sprite) {
        Editor::SpriteManager::init(obj, "preview", position, scale, -5, sprite);
        Editor::SpriteManager::set_color(obj, glm::vec4(1.0f, 1.0f, 1.0f, 0.0f));
        Editor::SpriteManager::set_saturation(obj, 0.3f);
        obj->pickable = false;
    }
}
