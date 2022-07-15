/**
 * @author Charlie Davidson 
 * Created on 7/10/22.
 */

#include "copied_objects.h"

#include "editor/game_object.h"
#include "editor/scene.h"
#include "editor/render/sprite_renderer.h"
#include "editor/collision/chunk_manager.h"
#include "selected_objects.h"
#include "object_manager.h"
#include "editor/sprite_system.h"
#include "drag_objects.h"

namespace Copy {

    static Editor::GameObject::game_object **copied_objects;
    static int copied_objects_count;
    static glm::vec2 copy_offset;
    static bool copied;

    static Editor::GameObject::game_object *game_objects;
    static int game_object_count;
    static Editor::GameObject::game_object **selected_objects;
    static int selected_object_count;

    /**
     * Initialize copied objects.
     */
    void init() {
        copied_objects = new Editor::GameObject::game_object *[1000];
        copied_objects_count = 0;
        copy_offset = glm::vec2 {0.1f, -0.1f};
        copied = false;
    }

    /**
     * Reset variables when the scene is reloaded.
     */
    void reload() {
        copied_objects_count = 0;
        copied = false;
    }

    /**
     * Get object information before updating.
     */
    void start_frame() {
        Editor::Scene::get_game_objects_list(&game_objects, &game_object_count);
        Selected::get_selected_objects(&selected_objects, &selected_object_count);
    }

    /**
     * Copy selected objects to the clipboard.
     */
    void copy_objects() {
        copied_objects_count = 0;
        for (int i = 0; i < selected_object_count; i++) {
            copied_objects[copied_objects_count] = selected_objects[i];
            copied_objects_count++;
        }
    }

    /**
     * Paste copied objects.
     */
    void paste_objects() {
        if (copied) {
            return;
        }
        copied = true;
        Selected::reset_selected();
        for (int i = 0; i < copied_objects_count; i++) {
            Editor::GameObject::game_object *copy;
            Editor::Scene::init_game_object(&copy);
            Editor::SpriteSystem::init_sprite_manager(&copy->spr_manager);
            *copy->spr_manager = *copied_objects[i]->spr_manager;
            copy->spr_manager->game_object = copy;
            Editor::SpriteManager::set_position(copy->spr_manager, copy->spr_manager->position + copy_offset);
            Editor::Scene::get_game_objects_list(&game_objects, &game_object_count);
            Editor::SpriteRenderer::add_sprite(copy->spr_manager);
            // Set the pasted objects to be selected
            Editor::GameObject::game_object *obj = &game_objects[game_object_count - 1];
            Editor::SpriteManager::set_selected(obj->spr_manager, true);
            Selected::get_selected_objects(&selected_objects, &selected_object_count);
            selected_objects[selected_object_count] = obj;
            Selected::set_selected_objects_count(selected_object_count + 1);
        }
    }

    /**
     * Delete selected objects.
     */
    void delete_objects() {
        for (int i = 0; i < selected_object_count; i++) {
            selected_objects[i]->dead = true;
            if (!Drag::is_invalid_placement()) {
                ChunkManager::set_solid_block(selected_objects[i]->spr_manager->grid_x,
                                              selected_objects[i]->spr_manager->grid_y, false);
            }
        }
        Editor::Scene::remove_game_objects();
        Selected::set_selected_objects_count(0);
        Editor::Scene::get_game_objects_list(&game_objects, &game_object_count);

        // Rebuffer every update batch (since pointers to game objects are now invalid)
        Editor::SpriteRenderer::clear_render_batches();
        for (int i = 0; i < game_object_count; i++) {
            Editor::SpriteRenderer::add_sprite(game_objects[i].spr_manager);
        }
        ObjectManager::reload();
    }

    /**
     * Get the copied objects.
     * @param objects Copied objects
     * @param object_count Copied object count
     */
    void get_copied_objects(Editor::GameObject::game_object ***objects, int *object_count) {
        *objects = copied_objects;
        *object_count = copied_objects_count;
    }

    /**
     * Get if the objets have been copied already.
     * @return True if already been copied
     */
    bool get_copied() {
        return copied;
    }

    /**
     * Set whether the objects can be copied again.
     * @param state False to reset
     */
    void set_copied(bool state) {
        copied = state;
    }
}
