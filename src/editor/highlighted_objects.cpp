/**
 * @author Charlie Davidson 
 * Created on 7/10/22.
 */

#include "editor/highlighted_objects.h"

#include "core/scene.h"
#include "core/imgui_layer.h"
#include "render/render.h"
#include "editor/object_manager.h"
#include "editor/mouse_drag_events.h"
#include "editor/selection_box.h"

namespace Highlight {

    static GameObject::game_object **highlighted_objects;
    static int highlighted_object_count;
    static GameObject::game_object *highlighted_obj;

    static GameObject::game_object *game_objects;
    static int game_object_count;
    static GameObject::game_object *selection_box;

    /**
     * Initialize highlighted objects.
     */
    void init() {
        highlighted_objects = new GameObject::game_object *[1000];
        highlighted_object_count = 0;
        highlighted_obj = nullptr;
    }

    /**
     * Reset variables when the scene is reloaded.
     */
    void reload() {
        highlighted_object_count = 0;
    }

    /**
     * Update highlighted objects.
     */
    void update() {
        Scene::get_game_objects_list(&game_objects, &game_object_count);
        SelectionBox::get_selection_box(&selection_box);
        Highlight::reset();
        Highlight::highlight_objects();
    }

    /**
     * Reset all objects to not be highlighted. Do this at the start of each frame.
     */
    static void reset() {
        // Set all game objects to not be highlighted or dragging
        for (int i = 0; i < game_object_count; i++) {
            GameObject::set_highlighted(&game_objects[i], false);
            GameObject::set_dragging(&game_objects[i], false);
        }
        // Only set the highlighted object to null if objects are not being dragged
        if (!Mouse::is_dragging_objects()) {
            highlighted_obj = nullptr;
        }
    }

    /**
     * Highlight objects, either under the mouse pointer or touching the selection box.
     */
    static void highlight_objects() {
        // Check if the mouse is being dragged or not
        if (Mouse::is_multiselect()) {
            // Highlight game objects intersecting with the selection box
            Render::highlight_game_objects(highlighted_objects,&highlighted_object_count, selection_box);
        } else {
            // Highlight the object that the mouse cursor is over
            if (!ImGuiLayer::want_mouse_capture() && !Mouse::is_dragging_objects()) {
                Render::highlight_game_object(&highlighted_obj);
            }
        }
    }

    /**
     * Get the highlighted object.
     * @param object Highlighted object
     */
    void get_highlighted_object(GameObject::game_object **object) {
        *object = highlighted_obj;
    }

    /**
     * Get the highlighted objects.
     * @param objects Highlighted objects
     * @param object_count Highlighted object count
     */
    void get_highlighted_objects(GameObject::game_object ***objects, int *object_count) {
        *objects = highlighted_objects;
        *object_count = highlighted_object_count;
    }
}
