/**
 * @author Charlie Davidson 
 * Created on 7/10/22.
 */

#include "editor/copied_objects.h"

#include "core/scene.h"
#include "render/render.h"
#include "editor/selected_objects.h"
#include "editor/object_manager.h"

namespace Copy {

    static GameObject::game_object **copied_objects;
    static int copied_objects_count;
    static glm::vec2 copy_offset;

    static GameObject::game_object *game_objects;
    static int game_object_count;
    static GameObject::game_object **selected_objects;
    static int selected_object_count;

    /**
     * Initialize copied objects.
     */
    void init() {
        copied_objects = new GameObject::game_object *[1000];
        copied_objects_count = 0;
        copy_offset = glm::vec2 {0.0f, 0.0f};
    }

    /**
     * Reset variables when the scene is reloaded.
     */
    void reload() {
        copied_objects_count = 0;
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
        copy_offset = glm::vec2 {0.1f, -0.1f};
    }

    /**
     * Paste copied objects.
     */
    void paste_objects() {
        Selected::reset_selected();
        for (int i = 0; i < copied_objects_count; i++) {
            GameObject::game_object copy = *copied_objects[i];
            GameObject::set_position(&copy,copy.position + copy_offset);
            Scene::add_game_object(&copy);
            Scene::get_game_objects_list(&game_objects, &game_object_count);
            // Set the pasted objects to be selected
            GameObject::game_object *obj = &game_objects[game_object_count - 1];
            GameObject::set_selected(obj, true);
            selected_objects[selected_object_count] = obj;
            selected_object_count++;
        }
        // Offset the copy position (for cascading effect with multiple pastes)
        copy_offset += glm::vec2 {0.1f, -0.1f};
    }

    /**
     * Delete selected objects.
     */
    void delete_objects() {
        for (int i = 0; i < selected_object_count; i++) {
            GameObject::set_dead(selected_objects[i], true);
        }
        Scene::remove_game_objects();
        selected_object_count = 0;
        Scene::get_game_objects_list(&game_objects, &game_object_count);

        // Rebuffer every render batch (since pointers to game objects are now invalid)
        Render::clear_render_batches();
        for (int i = 0; i < game_object_count; i++) {
            Render::add_game_object(&game_objects[i]);
        }
        ObjectManager::reload();
    }
}
