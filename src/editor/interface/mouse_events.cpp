/**
 * @author Charlie Davidson 
 * Created on 7/10/22.
 */

#include "mouse_events.h"

#include "selected_objects.h"
#include "highlighted_objects.h"
#include "selection_box.h"
#include "holding_object.h"
#include "copied_objects.h"
#include "drag_objects.h"

namespace Mouse {

    static bool mouse_down;

    /**
     * Handle mouse events.
     */
    void update() {
        //printf("Mouse update\n");
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
        if (Holding::is_holding()) {
            if (!mouse_down) {
                Holding::place_holding_object(true);
                mouse_down = true;
            }
            return;
        }
        if (Highlight::is_highlighted()) {
            if (Highlight::is_selected()) {
                // Start dragging selected object(s)
                Mouse::update_drag_objects();
            } else {
                // Select the highlighted object
                Selected::select_highlighted_object();
            }
        } else {
            // Start a multiselect action
            Mouse::update_multiselect();
        }
    }

    /**
     * Handle what happens when the mouse is shift-clicked.
     */
    static void shift_click() {
        if (Holding::is_holding()) {
            Holding::place_holding_object(false);
            mouse_down = true;
            return;
        }
        if (Highlight::is_highlighted()) {
            if (!mouse_down) {
                mouse_down = true;
                if (Highlight::is_selected()) {
                    // Shift clicking a selected object will unselect it
                    Selected::unselect_object();
                } else {
                    // Shift clicking an unselected object will select it
                    Selected::select_object();
                }
            }
        } else {
            // Start a multiselect action
            Mouse::update_multiselect();
        }
    }

    /**
     * Handle what happens when the mouse is released.
     */
    static void mouse_release() {
        //printf("Mouse release\n");
        // If the mouse was performing a multiselect action, releasing the mouse will select all highlighted objects
        if (SelectionBox::is_multiselect()) {
            Selected::select_highlighted_objects();
        }

        // If the mouse was dragging, end the drag action.
        if (Drag::is_dragging_objects()) {
            Drag::end_drag();
        }

        // Reset variables
        mouse_down = false;
        SelectionBox::hide();
        //printf("End mouse release\n");
    }

    /**
     * Drag selected objects.
     */
    void update_drag_objects() {
        // If not dragging, initiate a drag action
        if (!Drag::is_dragging_objects()) {
            mouse_down = true;
            Drag::start_drag();
        }
        // Update drag objects
        Drag::update_drag();
    }

    /**
     * Update the multiselect action.
     */
    void update_multiselect() {
        if (Copy::get_copied()) {
            Copy::delete_objects();
        }
        if (!Drag::is_dragging_objects()) {
            if (!SelectionBox::is_multiselect()) {
                // Starts a multiselect action
                if (!mouse_down) {
                    mouse_down = true;
                    Selected::reset_selected();
                    SelectionBox::start();
                }
            }
            // Scales selection box while multiselect is active
            SelectionBox::update();
        }
    }

    bool is_mouse_down() {
        return mouse_down;
    }
}
