/**
 * @author Charlie Davidson 
 * Created on 7/10/22.
 */

#include "editor/key_events.h"

#include "core/key_listener.h"
#include "editor/selected_objects.h"
#include "editor/copied_objects.h"
#include "editor/mouse_drag_events.h"
#include "editor/holding_object.h"

namespace Key {

    /**
     * Handle key events.
     */
    void update() {
        // Escape will deselect all game objects
        if (Key::get_key_pressed(GLFW_KEY_ESCAPE) && !Mouse::is_dragging_objects()) {
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

        // Delete holding object
        if (Holding::is_holding()) {
            if (Key::get_key_begin_press(GLFW_KEY_DELETE) || Key::get_key_begin_press(GLFW_KEY_BACKSPACE) || Key::get_key_pressed(GLFW_KEY_ESCAPE)) {
                Holding::destroy_holding_object();
            }
        }
    }
}
