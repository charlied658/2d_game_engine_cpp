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

    static GameObject::game_object **highlighted_objects;
    static GameObject::game_object **selected_objects;
    static GameObject::game_object *shadow_objects;

    static int highlighted_object_count = 0;
    static int selected_object_count = 0;
    static int shadow_object_count = 0;

    bool drag_objects;
    bool multiselect;
    bool shift_select;
    static double start_x, start_y;
    static glm::vec2 *obj_start_pos;

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
        obj_start_pos = new glm::vec2[1000];
        highlighted_object_count = 0;
        selected_object_count = 0;
        shadow_object_count = 0;

        // Generate null sprite (Renders as a solid color)
        sprite_null = Sprite::sprite {0, glm::vec2{0.0f, 0.0f}, glm::vec2{1.0f, 1.0f}};
        sprite_null.is_null = true;

        // Generate selection box (Appears when you click and drag to multiselect)
        GameObject::init(&selection_box, "selection_box", glm::vec2 {1.0f, 1.5f}, glm::vec2 {1.0f, 1.0f}, 2, &sprite_null);
        GameObject::set_color(&selection_box, glm::vec4 {1.0f, 1.0f, 0.0f, 0.2f});
        selection_box.pickable = false;
        selection_box.visible = true;
        Render::add_game_object(&selection_box);
    }

    /**
     * Perform mouse checks to select or unselect objects.
     * TODO: Reduce complexity of this file
     */
    void mouse_controls() {

        GameObject::game_object *game_objects;
        int game_object_count;
        Scene::get_game_objects_list(&game_objects, &game_object_count);

        // Reset all game objects to be not highlighted
        for (int i = 0; i < game_object_count; i++) {
            GameObject::set_highlighted(&game_objects[i], false);
            GameObject::set_dragging(&game_objects[i], false);
        }
        if (!drag_objects) {
            highlighted_obj = nullptr;
        }

        // If the mouse is not being dragged, highlight the object that the mouse cursor is over
        if (!multiselect && !drag_objects) {
            // Do not highlight an object if the mouse is over an ImGui window
            if (!ImGuiLayer::want_mouse_capture()) {
                Render::highlight_game_object(&highlighted_obj);
                if (highlighted_obj) {
                    GameObject::set_highlighted(highlighted_obj, true);
                }
            }
        }

        // If the mouse is being dragged, highlight the game objects intersecting with the selection box
        if (multiselect) {
            Render::highlight_game_objects(highlighted_objects,&highlighted_object_count,
                                           selection_box.position,selection_box.scale);
        }

        // When the mouse is released, select all highlighted objects
        if (!Mouse::get_mouse_pressed(GLFW_MOUSE_BUTTON_LEFT) && multiselect) {
            multiselect = false;
            if (highlighted_object_count > 0) {
                for (int i = 0; i < highlighted_object_count; i++) {
                    selected_objects[i] = highlighted_objects[i];
                    GameObject::set_selected(selected_objects[i], true);
                }
                selected_object_count = highlighted_object_count;
                highlighted_object_count = 0;
            }
        }

        // Clicking and dragging on selected objects will move them
        if (Mouse::get_mouse_pressed(GLFW_MOUSE_BUTTON_LEFT) && !Key::get_key_pressed(GLFW_KEY_LEFT_SHIFT)) {
            // If not dragging, initiate a drag action
            if (!drag_objects && highlighted_obj && highlighted_obj->selected) {
                drag_objects = true;
                start_x = Mouse::get_worldX();
                start_y = Mouse::get_worldY();
                for (int i = 0; i < selected_object_count; i++) {
                    obj_start_pos[i] = selected_objects[i]->position;
                }
            }
            // If there are multiple selected objects, dragging one will move all of them
            if (drag_objects) {
                for (int i = 0; i < selected_object_count; i++) {
                    GameObject::set_dragging(selected_objects[i], true);
                    GameObject::set_position(selected_objects[i], glm::vec2{
                            Mouse::get_worldX() - start_x + obj_start_pos[i].x,
                            Mouse::get_worldY() - start_y + +obj_start_pos[i].y});
                }
            }
        }

        // Clicking on an unselected object will select it
        if (Mouse::get_mouse_pressed(GLFW_MOUSE_BUTTON_LEFT) && !Key::get_key_pressed(GLFW_KEY_LEFT_SHIFT) && highlighted_obj && !highlighted_obj->selected && !drag_objects) {
            // Unselect all currently selected objects
            if (selected_object_count > 0) {
                for (int i = 0; i < selected_object_count; i++) {
                    GameObject::set_selected(selected_objects[i], false);
                }
            }
            selected_objects[0] = highlighted_obj;
            GameObject::set_selected(selected_objects[0], true);
            selected_object_count = 1;
        }

        // Shift clicking an unselected object will select it
        if (Mouse::get_mouse_pressed(GLFW_MOUSE_BUTTON_LEFT) && Key::get_key_pressed(GLFW_KEY_LEFT_SHIFT) && highlighted_obj && !highlighted_obj->selected) {
            if (shift_select) {
                selected_objects[selected_object_count] = highlighted_obj;
                highlighted_obj->selected = true;
                selected_object_count++;
                shift_select = false;
            }
        }

        // Shift clicking a selected object will unselect it
        if (Mouse::get_mouse_pressed(GLFW_MOUSE_BUTTON_LEFT) && Key::get_key_pressed(GLFW_KEY_LEFT_SHIFT) && highlighted_obj && highlighted_obj->selected) {
            if (shift_select) {
                // Remove it from the selected game objects
                bool found_obj = false;
                for (int i = 0; i < selected_object_count; i++) {
                    if (selected_objects[i] == highlighted_obj) {
                        GameObject::set_selected(highlighted_obj, false);
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
        }

        // Clicking and dragging on the background starts a multiselect action
        if (Mouse::get_mouse_pressed(GLFW_MOUSE_BUTTON_LEFT) && !drag_objects) {
            if (!multiselect && !highlighted_obj) {
                multiselect = true;
                start_x = Mouse::get_worldX();
                start_y = Mouse::get_worldY();
                GameObject::set_position(&selection_box, glm::vec2{start_x, start_y});
                selection_box.visible = true;
                GameObject::update_color(&selection_box);
                // Unselect all currently selected game objects
                if (selected_object_count > 0) {
                    for (int i = 0; i < selected_object_count; i++) {
                        GameObject::set_selected(selected_objects[i], false);
                    }
                    selected_object_count = 0;
                }
            }
            GameObject::set_scale(&selection_box,glm::vec2{
                    Mouse::get_worldX() - start_x, Mouse::get_worldY() - start_y});
        }

        // If the mouse is not pressed, reset variables
        if (!Mouse::get_mouse_pressed(GLFW_MOUSE_BUTTON_LEFT)) {
            drag_objects = false;
            multiselect = false;
            shift_select = true;
            selection_box.visible = false;
            GameObject::update_color(&selection_box);
        }

        // Render shadows for each selected game object
        for (int i = 0; i < selected_object_count; i++) {
            if (i < shadow_object_count) {
                shadow_objects[i].visible = true;
                GameObject::init(&shadow_objects[i], "shadow", selected_objects[i]->position + glm::vec2 {0.02f, -0.02f}, selected_objects[i]->scale, -5, &selected_objects[i]->sprite);
                GameObject::set_color(&shadow_objects[i], glm::vec4(0.0f, 0.0f, 0.0f, 0.7f));
                shadow_objects[i].pickable = false;
            } else {
                // Add new shadows if necessary
                shadow_objects[i] = GameObject::game_object {};
                GameObject::init(&shadow_objects[i], "shadow", selected_objects[i]->position + glm::vec2 {0.02f, -0.02f}, selected_objects[i]->scale, -5, &selected_objects[i]->sprite);
                GameObject::set_color(&shadow_objects[i], glm::vec4(0.0f, 0.0f, 0.0f, 0.7f));
                shadow_objects[i].pickable = false;
                Render::add_game_object(&shadow_objects[i]);
                shadow_object_count++;
            }
        }
        // Set the remaining shadows to be invisible
        if (shadow_object_count > selected_object_count) {
            for (int i = selected_object_count; i < shadow_object_count; i++) {
                shadow_objects[i].visible = false;
                GameObject::update_color(&shadow_objects[i]);
            }
        }

        // Escape will deselect all game objects
        if (Key::get_key_pressed(GLFW_KEY_ESCAPE) && !drag_objects) {
            if (selected_object_count > 0) {
                for (int i = 0; i < selected_object_count; i++) {
                    selected_objects[i]->selected = false;
                }
                selected_object_count = 0;
            }
        }

        // Space resets positions of game objects (temporary)
        if (Key::get_key_pressed(GLFW_KEY_SPACE) && !drag_objects) {
            GameObject::set_position(&game_objects[0], glm::vec2 {2.5f,1.0f});
            GameObject::set_position(&game_objects[1], glm::vec2 {3.0f,1.0f});
            GameObject::set_position(&game_objects[2], glm::vec2 {2.0f,1.0f});
        }
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
