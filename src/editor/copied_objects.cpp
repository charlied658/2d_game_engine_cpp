/**
 * @author Charlie Davidson 
 * Created on 7/10/22.
 */

#include "editor/copied_objects.h"

#include "core/scene.h"
#include "core/render/sprite_renderer.h"
#include "editor/collision/chunk_manager.h"
#include "editor/selected_objects.h"
#include "editor/object_manager.h"

namespace Copy {

    static SpriteManager::sprite_manager **copied_objects;
    static int copied_objects_count;
    static glm::vec2 copy_offset;
    static bool copied;

    static SpriteManager::sprite_manager *game_objects;
    static int game_object_count;
    static SpriteManager::sprite_manager **selected_objects;
    static int selected_object_count;

    /**
     * Initialize copied objects.
     */
    void init() {
        copied_objects = new SpriteManager::sprite_manager *[1000];
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
        Scene::get_game_objects_list(&game_objects, &game_object_count);
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
            SpriteManager::sprite_manager copy = *copied_objects[i];
            SpriteManager::set_position(&copy, copy.position + copy_offset);
            Scene::add_game_object(&copy);
            Scene::get_game_objects_list(&game_objects, &game_object_count);
            // Set the pasted objects to be selected
            SpriteManager::sprite_manager *obj = &game_objects[game_object_count - 1];
            SpriteManager::set_selected(obj, true);
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
            ChunkManager::set_solid_block(selected_objects[i]->grid_x, selected_objects[i]->grid_y, false);
        }
        Scene::remove_game_objects();
        Selected::set_selected_objects_count(0);
        Scene::get_game_objects_list(&game_objects, &game_object_count);

        // Rebuffer every update batch (since pointers to game objects are now invalid)
        SpriteRenderer::clear_render_batches();
        for (int i = 0; i < game_object_count; i++) {
            SpriteRenderer::add_sprite(&game_objects[i]);
        }
        ObjectManager::reload();
    }

    /**
     * Get the copied objects.
     * @param objects Copied objects
     * @param object_count Copied object count
     */
    void get_copied_objects(SpriteManager::sprite_manager ***objects, int *object_count) {
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
