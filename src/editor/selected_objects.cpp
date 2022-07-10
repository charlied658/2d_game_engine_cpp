/**
 * @author Charlie Davidson 
 * Created on 7/10/22.
 */

#include "editor/selected_objects.h"

#include "editor/highlighted_objects.h"
#include "editor/mouse_drag_events.h"

namespace Selected {

    static GameObject::game_object **selected_objects;
    static int selected_object_count;

    static GameObject::game_object **highlighted_objects;
    static int highlighted_object_count;
    static GameObject::game_object *highlighted_obj;

    /**
     * Initialize selected objects.
     */
    void init() {
        selected_objects = new GameObject::game_object *[1000];
        selected_object_count = 0;
    }

    /**
     * Reset variables when the scene is reloaded.
     */
    void reload() {
        selected_object_count = 0;
    }

    /**
     * Get object information before updating.
     */
    void start_frame() {
        Highlight::get_highlighted_objects(&highlighted_objects, &highlighted_object_count);
        Highlight::get_highlighted_object(&highlighted_obj);
    }

    /**
     * Reset all currently selected objects to be unselected.
     */
    void reset_selected() {
        if (selected_object_count > 0) {
            for (int i = 0; i < selected_object_count; i++) {
                selected_objects[i]->selected = false;
            }
            selected_object_count = 0;
        }
    }

    /**
     * Unselect one object.
     * @param obj Game object reference
     */
    void unselect_object(GameObject::game_object *obj) {
        bool found_obj = false;
        for (int i = 0; i < selected_object_count; i++) {
            if (selected_objects[i] == obj) {
                GameObject::set_selected(obj, false);
                Mouse::set_shift_click_down(true);
                found_obj = true;
            }
            if (found_obj && i < selected_object_count - 1) {
                selected_objects[i] = selected_objects[i + 1];
            }
        }
        if (found_obj) {
            selected_object_count--;
        }
    }

    /**
     * Select the currently highlighted object.
     */
    void select_object() {
        if (!Mouse::is_dragging_objects()) {
            Selected::reset_selected();
            selected_objects[0] = highlighted_obj;
            GameObject::set_selected(selected_objects[0], true);
            selected_object_count = 1;
        }
    }

    /**
     * Select all highlighted objects.
     */
    void select_objects() {
        if (highlighted_object_count > 0) {
            for (int i = 0; i < highlighted_object_count; i++) {
                selected_objects[i] = highlighted_objects[i];
                GameObject::set_selected(selected_objects[i], true);
            }
            selected_object_count = highlighted_object_count;
            highlighted_object_count = 0;
        }
    }

    /**
     * Get the selected objects.
     * @param objects Selected objects
     * @param object_count Selected object count
     */
    void get_selected_objects(GameObject::game_object ***objects, int *object_count) {
        *objects = selected_objects;
        *object_count = selected_object_count;
    }
}