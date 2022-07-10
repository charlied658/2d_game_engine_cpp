/**
 * @author Charlie Davidson 
 * Created on 7/9/22.
 */

#include "core/game_object.h"
#include "core/scene.h"
#include "core/imgui_layer.h"
#include "core/mouse_listener.h"
#include "core/key_listener.h"
#include "render/render.h"
#include "editor/select_objects.h"

namespace SelectObjects {

    static GameObject::game_object *game_objects;
    static GameObject::game_object **highlighted_objects;
    static GameObject::game_object **selected_objects;
    static GameObject::game_object *shadow_objects;
    static GameObject::game_object **copied_objects;

    static int highlighted_object_count = 0;
    static int selected_object_count = 0;
    static int shadow_object_count = 0;
    static int copied_objects_count = 0;
    static int game_object_count;

    static bool drag_objects;
    static bool multiselect;
    static bool shift_select;
    static double start_x, start_y;
    static glm::vec2 *obj_start_pos;
    static glm::vec2 copy_offset;

    static Sprite::sprite sprite_null;
    static GameObject::game_object selection_box;
    static GameObject::game_object *highlighted_obj;

    /**
     * Initialize objects.
     */
    void init() {
        highlighted_objects = new GameObject::game_object *[1000];
        selected_objects = new GameObject::game_object *[1000];
        shadow_objects = new GameObject::game_object[1000];
        copied_objects = new GameObject::game_object *[1000];

        highlighted_object_count = 0;
        selected_object_count = 0;
        shadow_object_count = 0;
        copied_objects_count = 0;

        obj_start_pos = new glm::vec2[1000];
        copy_offset = glm::vec2 {0.0f, 0.0f};

        // Generate null sprite (Renders as a solid color)
        Sprite::get_null_sprite(&sprite_null);

        // Generate selection box (Appears when you click and drag to multiselect)
        GameObject::init(&selection_box, "selection_box", glm::vec2 {1.0f, 1.5f}, glm::vec2 {1.0f, 1.0f}, 2, &sprite_null);
        GameObject::set_color(&selection_box, glm::vec4 {1.0f, 1.0f, 0.0f, 0.2f});
        GameObject::set_pickable(&selection_box, false);
        GameObject::set_visible(&selection_box, false);
        Render::add_game_object(&selection_box);
    }

    /**
     * Reset variables when the scene is reloaded.
     */
    void reload() {
        highlighted_object_count = 0;
        selected_object_count = 0;
        shadow_object_count = 0;
        copied_objects_count = 0;
        GameObject::set_visible(&selection_box, false);
        Render::add_game_object(&selection_box);
    }

    /**
     * Perform mouse checks to select or unselect objects.
     * TODO: Break this file up into multiple files
     */
    void update_selected_objects() {

        // Get game objects list from scene
        Scene::get_game_objects_list(&game_objects, &game_object_count);

        // Highlight objects under the mouse or within the selection box
        SelectObjects::reset_highlighted();
        SelectObjects::highlight_objects();

        // Handle clicking
        if (Mouse::get_mouse_pressed(GLFW_MOUSE_BUTTON_LEFT)) {
            if (Key::get_key_pressed(GLFW_KEY_LEFT_SHIFT)) {
                // Shift click
                SelectObjects::shift_click();
            } else {
                // Mouse click
                SelectObjects::mouse_click();
            }
        } else {
            // Mouse released
            SelectObjects::mouse_released();
        }

        // Render shadows under each selected game object
        render_shadows();

        // Escape will deselect all game objects
        if (Key::get_key_pressed(GLFW_KEY_ESCAPE) && !drag_objects) {
            SelectObjects::reset_selected();
        }

        // Key bindings for copy / paste
        if (Key::get_key_pressed(GLFW_KEY_LEFT_CONTROL) || Key::get_key_pressed(GLFW_KEY_LEFT_SUPER)) {
            if (Key::get_key_begin_press(GLFW_KEY_C)) {
                SelectObjects::copy_objects();
            } else if (Key::get_key_begin_press(GLFW_KEY_V)) {
                SelectObjects::paste_objects();
            }
        }

        // Delete selected objects
        if (Key::get_key_begin_press(GLFW_KEY_DELETE) || Key::get_key_begin_press(GLFW_KEY_BACKSPACE)) {
            SelectObjects::delete_objects();
        }
    }

    /**
     * Reset all objects to not be highlighted. Do this at the start of each frame.
     */
    void reset_highlighted() {
        // Set all game objects to not be highlighted or dragging
        for (int i = 0; i < game_object_count; i++) {
            GameObject::set_highlighted(&game_objects[i], false);
            GameObject::set_dragging(&game_objects[i], false);
        }
        // Only set the highlighted object to null if objects are not being dragged
        if (!drag_objects) {
            highlighted_obj = nullptr;
        }
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
                shift_select = false;
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
     * Highlight objects, either under the mouse pointer or touching the selection box
     */
    void highlight_objects() {
        // Check if the mouse is being dragged or not
        if (multiselect) {
            // Highlight game objects intersecting with the selection box
            Render::highlight_game_objects(highlighted_objects,&highlighted_object_count, &selection_box);
        } else {
            // Highlight the object that the mouse cursor is over
            if (!ImGuiLayer::want_mouse_capture() && !drag_objects) {
                Render::highlight_game_object(&highlighted_obj);
            }
        }
    }

    /**
     * Select the currently highlighted object.
     */
    void select_object() {
        if (!drag_objects) {
            SelectObjects::reset_selected();
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
     * Handle what happens when the mouse is clicked (not shift-clicked).
     */
    void mouse_click() {
        if (highlighted_obj) {
            if (highlighted_obj->selected) {
                // Start dragging selected object(s)
                SelectObjects::move_objects();
            } else {
                // Select the highlighted object
                SelectObjects::select_object();
            }
        } else {
            // Start a multiselect action
            SelectObjects::start_multiselect();
        }
    }

    /**
     * Handle what happens when the mouse is shift-clicked.
     */
    void shift_click() {
        if (highlighted_obj && shift_select) {
            if (highlighted_obj->selected) {
                // Shift clicking a selected object will unselect it
                SelectObjects::unselect_object(highlighted_obj);
            } else {
                // Shift clicking an unselected object will select it
                selected_objects[selected_object_count] = highlighted_obj;
                GameObject::set_selected(highlighted_obj, true);
                selected_object_count++;
                shift_select = false;
            }
        }
    }

    /**
     * Handle what happens when the mouse is released.
     */
    void mouse_released() {
        // If the mouse was performing a multiselect action, releasing the mouse will select all highlighted objects
        if (multiselect) {
            multiselect = false;
            SelectObjects::select_objects();
        }
        // Reset variables
        drag_objects = false;
        multiselect = false;
        shift_select = true;
        GameObject::set_visible(&selection_box, false);
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
        if (!drag_objects && shift_select) {
            if (!multiselect) {
                multiselect = true;
                start_x = Mouse::get_worldX();
                start_y = Mouse::get_worldY();
                GameObject::set_position(&selection_box, glm::vec2{start_x, start_y});
                GameObject::set_visible(&selection_box, true);
                SelectObjects::reset_selected();
            }
            glm::vec2 new_scale {Mouse::get_worldX() - start_x, Mouse::get_worldY() - start_y};
            GameObject::set_scale(&selection_box,new_scale);
        }
    }

    /**
     * Render shadows under each selected game object.
     */
    void render_shadows() {
        // Render shadows for each selected game object
        for (int i = 0; i < selected_object_count; i++) {
            if (i < shadow_object_count) {
                GameObject::set_visible(&shadow_objects[i], true);
                GameObject::init(&shadow_objects[i], "shadow", selected_objects[i]->position + glm::vec2 {0.02f, -0.02f}, selected_objects[i]->scale, -5, &selected_objects[i]->sprite);
                GameObject::set_color(&shadow_objects[i], glm::vec4(0.0f, 0.0f, 0.0f, 0.7f));
                GameObject::set_pickable(&shadow_objects[i], false);
            } else {
                // Add new shadows if necessary
                shadow_objects[i] = GameObject::game_object {};
                GameObject::init(&shadow_objects[i], "shadow", selected_objects[i]->position + glm::vec2 {0.02f, -0.02f}, selected_objects[i]->scale, -5, &selected_objects[i]->sprite);
                GameObject::set_color(&shadow_objects[i], glm::vec4(0.0f, 0.0f, 0.0f, 0.7f));
                GameObject::set_pickable(&shadow_objects[i], false);
                Render::add_game_object(&shadow_objects[i]);
                shadow_object_count++;
            }
        }
        // Set the remaining shadows to be invisible
        if (shadow_object_count > selected_object_count) {
            for (int i = selected_object_count; i < shadow_object_count; i++) {
                GameObject::set_visible(&shadow_objects[i], false);
            }
        }
    }

    /**
     * Copy selected objects to the clipboard.
     */
    void copy_objects() {
        copied_objects_count = 0;
        for (int i = 0; i < selected_object_count; i++) {
            copied_objects[copied_objects_count] = selected_objects[i];
            copied_objects_count++;
        }
        copy_offset = glm::vec2 {0.1f, -0.1f};
    }

    /**
     * Paste copied objects.
     */
    void paste_objects() {
        SelectObjects::reset_selected();
        for (int i = 0; i < copied_objects_count; i++) {
            GameObject::game_object copy = *copied_objects[i];
            GameObject::set_position(&copy,copy.position + copy_offset);
            Scene::add_game_object(&copy);
            Scene::get_game_objects_list(&game_objects, &game_object_count);
            // Set the pasted objects to be selected
            GameObject::game_object *obj = &game_objects[game_object_count - 1];
            GameObject::set_selected(obj, true);
            selected_objects[selected_object_count] = obj;
            selected_object_count++;
        }
        // Offset the copy position (for cascading effect with multiple pastes)
        copy_offset += glm::vec2 {0.1f, -0.1f};
    }

    /**
     * Delete selected objects.
     */
    void delete_objects() {
        for (int i = 0; i < selected_object_count; i++) {
            GameObject::set_dead(selected_objects[i], true);
        }
        Scene::remove_game_objects();
        selected_object_count = 0;
        Scene::get_game_objects_list(&game_objects, &game_object_count);

        // Rebuffer every render batch (since pointers to game objects are now invalid)
        Render::clear_render_batches();
        for (int i = 0; i < game_object_count; i++) {
            Render::add_game_object(&game_objects[i]);
        }
        SelectObjects::reload();
    }

    /**
     * User interface elements.
     */
    void imgui() {
        // Debug
        if (highlighted_obj) {
            ImGui::Text("Highlighted object: %s", highlighted_obj->name.c_str());
        }
        // Display highlighted game objects
        for (int i = 0; i < selected_object_count; i++) {
            ImGui::Text("Selected: %s", selected_objects[i]->name.c_str());
        }
    }
}
