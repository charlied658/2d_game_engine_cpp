/**
 * @author Charlie Davidson 
 * Created on 7/10/22.
 */

#include "editor/selected_objects.h"

#include "editor/highlighted_objects.h"
#include "editor/drag_objects.h"

namespace Selected {

    static SpriteManager::sprite_manager **selected_objects;
    static int selected_object_count;

    static SpriteManager::sprite_manager **highlighted_objects;
    static int highlighted_object_count;
    static SpriteManager::sprite_manager *highlighted_obj;

    /**
     * Initialize selected objects.
     */
    void init() {
        selected_objects = new SpriteManager::sprite_manager *[1000];
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

        for (int i = 0; i < selected_object_count; i++) {
            selected_objects[i]->new_z_index = 5;
        }
    }

    /**
     * Reset all currently selected objects to be unselected.
     */
    void reset_selected() {
        if (selected_object_count > 0) {
            for (int i = 0; i < selected_object_count; i++) {
                SpriteManager::set_selected(selected_objects[i], false);
            }
            selected_object_count = 0;
        }
    }

    /**
     * Select one object.
     */
    void select_object() {
        selected_objects[selected_object_count] = highlighted_obj;
        SpriteManager::set_selected(highlighted_obj, true);
        selected_object_count++;
    }

    /**
     * Unselect one object.
     */
    void unselect_object() {
        bool found_obj = false;
        for (int i = 0; i < selected_object_count; i++) {
            if (selected_objects[i] == highlighted_obj) {
                SpriteManager::set_selected(highlighted_obj, false);
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
    void select_highlighted_object() {
        if (!Drag::is_dragging_objects()) {
            Selected::reset_selected();
            selected_objects[0] = highlighted_obj;
            SpriteManager::set_selected(selected_objects[0], true);
            selected_object_count = 1;
        }
    }

    /**
     * Select all highlighted objects.
     */
    void select_highlighted_objects() {
        if (highlighted_object_count > 0) {
            for (int i = 0; i < highlighted_object_count; i++) {
                selected_objects[i] = highlighted_objects[i];
                SpriteManager::set_selected(selected_objects[i], true);
            }
            selected_object_count = highlighted_object_count;
            highlighted_object_count = 0;
        }
    }

    /**
     * Select the holding object when it is placed.
     */
    void select_holding_object(SpriteManager::sprite_manager *obj) {
        reset_selected();
        selected_objects[0] = obj;
        selected_object_count = 1;
    }

    /**
     * Get the selected objects.
     * @param objects Selected objects
     * @param object_count Selected object count
     */
    void get_selected_objects(SpriteManager::sprite_manager ***objects, int *object_count) {
        *objects = selected_objects;
        *object_count = selected_object_count;
    }

    /**
     * Set the selected objects count.
     * @param Selected objects count
     */
    void set_selected_objects_count(int count) {
        selected_object_count = count;
    }
}
