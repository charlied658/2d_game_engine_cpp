/**
 * @author Charlie Davidson 
 * Created on 7/10/22.
 */

#include "key_events.h"

#include "core/key_listener.h"
#include "selected_objects.h"
#include "copied_objects.h"
#include "holding_object.h"
#include "drag_objects.h"

namespace Key {

    /**
     * Handle key events.
     */
    void update() {

        // Delete holding object
        if (Holding::is_holding()) {
            if (Key::get_key_begin_press(GLFW_KEY_DELETE) || Key::get_key_begin_press(GLFW_KEY_BACKSPACE) || Key::get_key_pressed(GLFW_KEY_ESCAPE)) {
                Holding::destroy_holding_object();
                return;
            }
        }

        // Escape will deselect all game objects
        if (Key::get_key_pressed(GLFW_KEY_ESCAPE) && !Drag::is_dragging_objects()) {
            if (Copy::get_copied()) {
                Copy::delete_objects();
            }
            Selected::reset_selected();
        }

        // Key bindings for copy / paste
        if (Key::get_key_pressed(GLFW_KEY_LEFT_CONTROL) || Key::get_key_pressed(GLFW_KEY_LEFT_SUPER)) {
            if (Key::get_key_begin_press(GLFW_KEY_C)) {
                Copy::copy_objects();
            } else if (Key::get_key_begin_press(GLFW_KEY_V)) {
                Copy::paste_objects();
            }
        }

        // Delete selected objects
        if (Key::get_key_begin_press(GLFW_KEY_DELETE) || Key::get_key_begin_press(GLFW_KEY_BACKSPACE)) {
            Copy::delete_objects();
        }
    }
}
