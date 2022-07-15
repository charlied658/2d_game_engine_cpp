/**
 * @author Charlie Davidson 
 * Created on 7/10/22.
 */

#include "selected_objects.h"

#include "editor/game_object.h"
#include "highlighted_objects.h"
#include "drag_objects.h"

namespace Selected {

    static Editor::GameObject::game_object **selected_objects;
    static int selected_object_count;

    static Editor::GameObject::game_object **highlighted_objects;
    static int highlighted_object_count;
    static Editor::GameObject::game_object *highlighted_obj;

    /**
     * Initialize selected objects.
     */
    void init() {
        selected_objects = new Editor::GameObject::game_object *[1000];
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
        //printf("Start frame selected\n");
        Highlight::get_highlighted_objects(&highlighted_objects, &highlighted_object_count);
        Highlight::get_highlighted_object(&highlighted_obj);

        for (int i = 0; i < selected_object_count; i++) {
            selected_objects[i]->spr_manager->new_z_index = 5;
        }
        //printf("End frame selected\n");
    }

    /**
     * Reset all currently selected objects to be unselected.
     */
    void reset_selected() {
        if (selected_object_count > 0) {
            for (int i = 0; i < selected_object_count; i++) {
                Editor::SpriteManager::set_selected(selected_objects[i]->spr_manager, false);
            }
            selected_object_count = 0;
        }
    }

    /**
     * Select one object.
     */
    void select_object() {
        selected_objects[selected_object_count] = highlighted_obj;
        Editor::SpriteManager::set_selected(highlighted_obj->spr_manager, true);
        selected_object_count++;
    }

    /**
     * Unselect one object.
     */
    void unselect_object() {
        bool found_obj = false;
        for (int i = 0; i < selected_object_count; i++) {
            if (selected_objects[i] == highlighted_obj) {
                Editor::SpriteManager::set_selected(highlighted_obj->spr_manager, false);
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
            Editor::SpriteManager::set_selected(selected_objects[0]->spr_manager, true);
            selected_object_count = 1;
        }
    }

    /**
     * Select all highlighted objects.
     */
    void select_highlighted_objects() {
        //printf("Begin select\n");
        if (highlighted_object_count > 0) {
            for (int i = 0; i < highlighted_object_count; i++) {
                //printf("Selecting object %d\n",i);
                selected_objects[i] = highlighted_objects[i];
                //printf("pointer: %p\n", (void *)&selected_objects[i]->spr_manager);
                Editor::SpriteManager::set_selected(selected_objects[i]->spr_manager, true);
            }
            selected_object_count = highlighted_object_count;
            //printf("Selected objects: %d\n", selected_object_count);
            Highlight::reload();
        }
    }

    /**
     * Select the holding object when it is placed.
     */
    void select_holding_object(Editor::GameObject::game_object *obj) {
        reset_selected();
        selected_objects[0] = obj;
        selected_object_count = 1;
    }

    /**
     * Get the selected objects.
     * @param objects Selected objects
     * @param object_count Selected object count
     */
    void get_selected_objects(Editor::GameObject::game_object ***objects, int *object_count) {
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
