/**
 * @author Charlie Davidson 
 * Created on 7/10/22.
 */

#include "editor/mouse_events.h"

#include "editor/object_manager.h"
#include "editor/selected_objects.h"
#include "editor/highlighted_objects.h"
#include "editor/selection_box.h"
#include "editor/holding_object.h"
#include "editor/copied_objects.h"
#include "util/collision_math.h"

namespace Mouse {

    static GameObject::game_object *highlighted_obj;
    static GameObject::game_object **selected_objects;
    static int selected_object_count;
    static GameObject::game_object *selection_box;

    static bool drag_objects;
    static bool multiselect;
    static bool shift_click_down;
    static double start_x, start_y;
    static glm::vec2 *obj_start_pos;

    /**
     * Initialize mouse objects.
     */
    void init() {
        obj_start_pos = new glm::vec2[1000];
    }

    /**
     * Get object information before updating.
     */
    void start_frame() {
        Selected::get_selected_objects(&selected_objects, &selected_object_count);
        SelectionBox::get_selection_box(&selection_box);
    }

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
        if (Holding::is_holding() && !Mouse::is_shift_click_down()) {
            Mouse::set_shift_click_down(true);
            Holding::place_holding_object(true);
            return;
        }
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
            Mouse::start_multiselect();
        }
    }

    /**
     * Handle what happens when the mouse is shift-clicked.
     */
    static void shift_click() {
        if (Holding::is_holding() && !Mouse::is_shift_click_down()) {
            Mouse::set_shift_click_down(true);
            Holding::place_holding_object(false);
            return;
        }
        if (highlighted_obj) {
            if (!Mouse::is_shift_click_down()) {
                Mouse::set_shift_click_down(true);
                if (highlighted_obj->selected) {
                    // Shift clicking a selected object will unselect it
                    Selected::unselect_object(highlighted_obj);
                }
                else {
                    // Shift clicking an unselected object will select it
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

        // If the mouse was dragging, snap objects to grid
        if (Mouse::is_dragging_objects()) {
            for (int i = 0; i < selected_object_count; i++) {
                glm::vec2 centered_position = {selected_objects[i]->position.x + 0.25f / 2,
                                               selected_objects[i]->position.y + 0.25f / 2};
                glm::vec2 position = {centered_position.x - Math::f_mod(centered_position.x, 0.25f),
                                      centered_position.y - Math::f_mod(centered_position.y, 0.25f)};
                GameObject::set_position(selected_objects[i], position);
            }
            Mouse::set_dragging_objects(false);
        }

        // Reset variables
        Mouse::set_multiselect(false);
        Mouse::set_dragging_objects(false);
        Mouse::set_shift_click_down(false);
        GameObject::set_visible(selection_box, false);
    }

    /**
     * Move selected objects.
     */
    void move_objects() {
        if (Copy::get_copied()) {
            Copy::set_copied(false);
        }

        // If not dragging, initiate a drag action
        if (!drag_objects) {
            drag_objects = true;
            start_x = Mouse::get_worldX();
            start_y = Mouse::get_worldY();
            for (int i = 0; i < selected_object_count; i++) {
                obj_start_pos[i] = selected_objects[i]->position;
            }
        } else {
            // If there are multiple selected objects, dragging one will move all of them
            for (int i = 0; i < selected_object_count; i++) {
                GameObject::set_dragging(selected_objects[i], true);
                GameObject::set_position(selected_objects[i], glm::vec2{
                        Mouse::get_worldX() - start_x + obj_start_pos[i].x,
                        Mouse::get_worldY() - start_y + +obj_start_pos[i].y});
            }
        }
    }

    /**
     * Begin a multiselect action.
     */
    void start_multiselect() {
        if (Copy::get_copied()) {
            Copy::delete_objects();
        }
        if (!drag_objects) {
            if (!multiselect) {
                // Starts a multiselect action
                if (!shift_click_down) {
                    multiselect = true;
                    shift_click_down = true;
                    start_x = Mouse::get_worldX();
                    start_y = Mouse::get_worldY();
                    GameObject::set_position(selection_box, glm::vec2{ start_x, start_y });
                    GameObject::set_visible(selection_box, true);
                    Selected::reset_selected();
                }
            }
            // Scales selection box while multiselect is active
            glm::vec2 new_scale{ Mouse::get_worldX() - start_x, Mouse::get_worldY() - start_y };
            GameObject::set_scale(selection_box, new_scale);
        }
    }

    /**
     * Check if the mouse is performing a multiselect action.
     * @return True if multiselect is active
     */
    bool is_multiselect() {
        return multiselect;
    }

    /**
     * Check if the mouse is dragging objects.
     * @return True if mouse is dragging
     */
    bool is_dragging_objects() {
        return drag_objects;
    }

    /**
     * Check if the mouse is currently shift-clicking
     * @return True if shift clicking
     */
    bool is_shift_click_down() {
        return shift_click_down;
    }

    /**
     * Set whether the mouse is performing a multiselect action.
     * @param state Set multiselect
     */
    void set_multiselect(bool state) {
        multiselect = state;
    }

    /**
     * Set whether the mouse is dragging objects.
     * @param state Set dragging
     */
    void set_dragging_objects(bool state) {
        drag_objects = state;
    }

    /**
     * Set whether the mouse is shift-clicking.
     * @param state Set shift-clicking
     */
    void set_shift_click_down(bool state) {
        shift_click_down = state;
    }
}
