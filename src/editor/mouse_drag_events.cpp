/**
 * @author Charlie Davidson 
 * Created on 7/10/22.
 */

#include "editor/mouse_drag_events.h"

#include "core/game_object.h"
#include "core/mouse_listener.h"
#include "editor/selected_objects.h"
#include "editor/selection_box.h"

namespace Mouse {

    static bool drag_objects;
    static bool multiselect;
    static bool shift_click_down;
    static double start_x, start_y;
    static glm::vec2 *obj_start_pos;

    static GameObject::game_object **selected_objects;
    static int selected_object_count;
    static GameObject::game_object *selection_box;

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
     * Move selected objects.
     */
    void move_objects() {
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
        // Clicking and dragging on the background starts a multiselect action
        if (!drag_objects && !shift_click_down) {
            if (!multiselect) {
                multiselect = true;
                start_x = Mouse::get_worldX();
                start_y = Mouse::get_worldY();
                GameObject::set_position(selection_box, glm::vec2{start_x, start_y});
                GameObject::set_visible(selection_box, true);
                Selected::reset_selected();
            }
            glm::vec2 new_scale {Mouse::get_worldX() - start_x, Mouse::get_worldY() - start_y};
            GameObject::set_scale(selection_box,new_scale);
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
