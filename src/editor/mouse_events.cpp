/**
 * @author Charlie Davidson 
 * Created on 7/10/22.
 */

#include "editor/mouse_events.h"

#include "editor/object_manager.h"
#include "editor/mouse_drag_events.h"
#include "editor/selected_objects.h"
#include "editor/highlighted_objects.h"
#include "editor/selection_box.h"

namespace Mouse {

    static GameObject::game_object *highlighted_obj;
    static GameObject::game_object **selected_objects;
    static int selected_object_count;
    static GameObject::game_object *selection_box;

    /**
     * Handle mouse events.
     */
    void update() {

        Highlight::get_highlighted_object(&highlighted_obj);
        Selected::get_selected_objects(&selected_objects, &selected_object_count);
        SelectionBox::get_selection_box(&selection_box);

        if (Mouse::get_mouse_pressed(GLFW_MOUSE_BUTTON_LEFT)) {
            if (Key::get_key_pressed(GLFW_KEY_LEFT_SHIFT)) {
                // Shift click
                Mouse::shift_click();
            } else {
                // Mouse click
                Mouse::mouse_click();
            }
        } else {
            // Mouse released
            Mouse::mouse_release();
        }
    }

    /**
     * Handle what happens when the mouse is clicked (not shift-clicked).
     */
    static void mouse_click() {
        if (highlighted_obj) {
            if (highlighted_obj->selected) {
                // Start dragging selected object(s)
                Mouse::move_objects();
            } else {
                // Select the highlighted object
                Selected::select_object();
            }
        } else {
            // Start a multiselect action
            // TODO: Should the mouse drag the camera instead?
            Mouse::start_multiselect();
        }
    }

    /**
     * Handle what happens when the mouse is shift-clicked.
     */
    static void shift_click() {
        if (highlighted_obj) {
            if (!Mouse::is_shift_click_down()) {
                Mouse::set_shift_click_down(true);
                if (highlighted_obj->selected) {
                    // Shift clicking a selected object will unselect it
                    printf("Unselected object\n");
                    Selected::unselect_object(highlighted_obj);
                }
                else {
                    // Shift clicking an unselected object will select it
                    printf("Selected object\n");
                    selected_objects[selected_object_count] = highlighted_obj;
                    GameObject::set_selected(highlighted_obj, true);
                    Selected::set_selected_objects_count(selected_object_count + 1);
                }
            }
        }
        else {
            // Start a multiselect action
            Mouse::start_multiselect();
        }
    }

    /**
     * Handle what happens when the mouse is released.
     */
    static void mouse_release() {
        // If the mouse was performing a multiselect action, releasing the mouse will select all highlighted objects
        if (Mouse::is_multiselect()) {
            Mouse::set_multiselect(false);
            Selected::select_objects();
        }
        // Reset variables
        Mouse::set_multiselect(false);
        Mouse::set_dragging_objects(false);
        Mouse::set_shift_click_down(false);
        GameObject::set_visible(selection_box, false);
    }
}
