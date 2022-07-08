/**
 * @author Charlie Davidson
 * Created on 7/5/22.
 */

#include <cstdio>
#include "core/scene.h"

#include "imgui/imgui.h"

#include "render/texture.h"
#include "render/render.h"
#include "core/game_object.h"
#include "core/sprite.h"
#include "core/spritesheet.h"
#include "core/camera.h"
#include "core/mouse_listener.h"
#include "core/imgui_layer.h"

namespace Scene {

    static GameObject::game_object *game_objects;
    static int game_object_count = 0;

    static GameObject::game_object **selected_objects;
    static int selected_object_count = 0;

    static GameObject::game_object **active_objects;
    static int active_object_count = 0;

    static GameObject::game_object *shadow_objects;
    static int shadow_object_count = 0;

    static unsigned int textureID1, textureID2;
    static Spritesheet::spritesheet spritesheet1, spritesheet2;
    static Sprite::sprite sprite1, sprite2, sprite3, sprite4;
    static Sprite::sprite sprite_null;
    static GameObject::game_object obj1, obj2, obj3;
    static GameObject::game_object selection_box;
    static GameObject::game_object *selected_obj;

    bool obj_drag;
    bool select_drag;
    bool shift_select;
    static double start_x, start_y;
    static glm::vec2 *obj_start_pos;

    static float color[4] = {1.0f, 1.0f, 1.0f, 1.0f};
    static bool reset_color = false;

    /**
     * Initialize the scene.
     */
    void init() {

        game_objects = new GameObject::game_object[1000];
        selected_objects = new GameObject::game_object *[1000];
        active_objects = new GameObject::game_object *[1000];
        shadow_objects = new GameObject::game_object[1000];

        obj_start_pos = new glm::vec2[1000];

        // Generate textures
        textureID1 = Texture::get_texture("assets/images/spritesheet.png")->textureID;
        textureID2 = Texture::get_texture("assets/images/turtle.png")->textureID;

        // Create spritesheet
        spritesheet1 = Spritesheet::spritesheet {14,2,textureID1};
        spritesheet2 = Spritesheet::spritesheet {4,1,textureID2};

        // Create sprites
        sprite1 = Spritesheet::get_sprite(&spritesheet1, 0);
        sprite2 = Spritesheet::get_sprite(&spritesheet1, 14);
        sprite3 = Spritesheet::get_sprite(&spritesheet2, 0);
        sprite_null = Sprite::sprite {0, glm::vec2{0.0f, 0.0f}, glm::vec2{1.0f, 1.0f}};
        sprite_null.is_null = true;

        // Generate game objects
        GameObject::init(&obj1, "obj1", glm::vec2 {2.5f, 1.0f}, glm::vec2 {0.5f, 0.5f}, 1, &sprite1);
        GameObject::init (&obj2, "obj2", glm::vec2 {3.0f, 1.0f}, glm::vec2 {0.5f, 0.5f}, 0, &sprite2);
        GameObject::init (&obj3, "obj3", glm::vec2 {2.0f, 1.0f}, glm::vec2 {0.5f, 0.75f}, 0, &sprite3);

        GameObject::init(&selection_box, "selection_box", glm::vec2 {1.0f, 1.5f}, glm::vec2 {1.0f, 1.0f}, 2, &sprite_null);
        GameObject::set_color(&selection_box, glm::vec4 {1.0f, 1.0f, 0.0f, 0.2f});
        selection_box.pickable = false;
        selection_box.visible = false;

        // Add game objects to the scene
        Scene::add_game_object(&obj1);
        Scene::add_game_object(&obj2);
        Scene::add_game_object(&obj3);
        Scene::add_game_object(&selection_box);

    }

    /**
     * Update the scene each frame.
     * @param dt Delta time
     */
    void update(double dt) {
        // Camera controls
        Camera::update(dt);

        // Update mouse position
        Mouse::calculate_world_coords();

        // Space will reset position of objects
        if (Key::get_key_pressed(GLFW_KEY_SPACE)) {
            GameObject::set_position(&obj1, glm::vec2 {2.5f, 1.0f});
            GameObject::set_position(&obj2, glm::vec2 {3.0f, 1.0f});
            GameObject::set_position(&obj3, glm::vec2 {2.0f, 1.0f});
        }

        // Escape will deselect all game objects
        if (Key::get_key_pressed(GLFW_KEY_ESCAPE)) {
            if (active_object_count > 0) {
                for (int i = 0; i  < active_object_count; i++) {
                    active_objects[i]->active = false;
                }
                active_object_count = 0;
            }
        }

        // Render shadow for each active game object
        for (int i = 0; i < active_object_count; i++) {
            if (i < shadow_object_count) {
                shadow_objects[i].visible = true;
                GameObject::init(&shadow_objects[i], "shadow", active_objects[i]->position + glm::vec2 {0.02f, -0.02f}, active_objects[i]->scale, -5, &active_objects[i]->sprite);
                GameObject::set_color(&shadow_objects[i], glm::vec4(0.0f, 0.0f, 0.0f, 0.7f));
            } else {
                // Add new shadows if necessary
                shadow_objects[i] = GameObject::game_object {};
                shadow_objects[i].pickable = false;
                GameObject::init(&shadow_objects[i], "shadow", active_objects[i]->position + glm::vec2 {0.02f, -0.02f}, active_objects[i]->scale, -5, &active_objects[i]->sprite);
                GameObject::set_color(&shadow_objects[i], glm::vec4(0.0f, 0.0f, 0.0f, 0.7f));
                add_game_object(&shadow_objects[i]);
                shadow_object_count++;
            }
        }
        // Set the remaining shadows to be invisible
        if (shadow_object_count > active_object_count) {
            for (int i = active_object_count; i < shadow_object_count; i++) {
                shadow_objects[i].visible = false;
                GameObject::update_color(&shadow_objects[i]);
            }
        }

        // Select one game object based on mouse position
        if (!select_drag && !ImGuiLayer::want_mouse_capture()) {
            if (!Mouse::get_mouse_pressed(GLFW_MOUSE_BUTTON_LEFT)) {
                Render::select_game_object(&selected_obj);
            }
        } else {
            if (selected_obj) {
                selected_obj->selected = false;
                GameObject::update_color(selected_obj);
                selected_obj = nullptr;
            }
        }

        // Select multiple objects if the mouse is being dragged
        if (select_drag) {
            if (active_object_count > 0) {
                for (int i = 0; i  < active_object_count; i++) {
                    active_objects[i]->active = false;
                }
                active_object_count = 0;
            }
            Render::select_game_objects(selected_objects, &selected_object_count, selection_box.position, selection_box.scale);
        } else {
            // Activate all selected objects when mouse is released
            if (selected_object_count > 0) {
                for (int i = 0; i < selected_object_count; i++) {
                    active_objects[i] = selected_objects[i];
                    active_objects[i]->active = true;
                }
                active_object_count = selected_object_count;
                selected_object_count = 0;
            }
        }

        // Handle what happens when the mouse is down
        if (Mouse::get_mouse_pressed(GLFW_MOUSE_BUTTON_LEFT)) {
            if (selected_obj) {
                if (Key::get_key_pressed(GLFW_KEY_LEFT_SHIFT)) {
                    if (shift_select) {
                        // Shift clicking an unselected object will select it
                        if (!selected_obj->active) {
                            active_objects[active_object_count] = selected_obj;
                            selected_obj->active = true;
                            active_object_count++;
                            shift_select = false;
                        } else {
                            // Shift clicking a selected object will unselect it
                            bool found_obj = false;
                            // Remove it from the active game objects
                            for (int i = 0; i < active_object_count; i++) {
                                if (active_objects[i] == selected_obj) {
                                    selected_obj->active = false;
                                    shift_select = false;
                                    found_obj = true;
                                }
                                if (found_obj && i < active_object_count - 1) {
                                    active_objects[i] = active_objects[i + 1];
                                }
                            }
                            if (found_obj) {
                                active_object_count--;
                            }
                        }
                    }
                } else {
                    // Clicking and dragging on a selected object will move it around
                    if (selected_obj->active) {
                        if (!obj_drag) {
                            obj_drag = true;
                            start_x = Mouse::get_worldX();
                            start_y = Mouse::get_worldY();
                            for (int i = 0; i < active_object_count; i++) {
                                obj_start_pos[i] = active_objects[i]->position;
                            }
                        }
                        // Move all the selected game objects
                        for (int i = 0; i < active_object_count; i++) {
                            GameObject::set_position(active_objects[i],
                                                     glm::vec2{Mouse::get_worldX() - start_x + obj_start_pos[i].x,
                                                               Mouse::get_worldY() - start_y + +obj_start_pos[i].y});
                        }
                    } else {
                        // Clicking on an unselected object will select it
                        if (active_object_count > 0) {
                            for (int i = 0; i < active_object_count; i++) {
                                active_objects[i]->active = false;
                            }
                        }
                        active_objects[0] = selected_obj;
                        active_objects[0]->active = true;
                        active_object_count = 1;
                    }
                }
            } else {
                // Clicking and dragging on the background starts a multiselect action
                if (!select_drag) {
                    select_drag = true;
                    start_x = Mouse::get_worldX();
                    start_y = Mouse::get_worldY();
                    GameObject::set_position(&selection_box, glm::vec2 {start_x, start_y});
                    selection_box.visible = true;
                    GameObject::update_color(&selection_box);
                }
                GameObject::set_scale(&selection_box, glm::vec2 {Mouse::get_worldX() - start_x, Mouse::get_worldY() - start_y});
            }
        } else {
            // If the mouse is not pressed, reset variables
            obj_drag = false;
            select_drag = false;
            shift_select = true;
            selection_box.visible = false;
            GameObject::update_color(&selection_box);
        }
    }

    /**
     * Update scene ImGui.
     */
    void imgui() {
        ImGui::Begin("Main Menu", nullptr, ImGuiWindowFlags_MenuBar);
        if (ImGui::BeginMenuBar()) {
            // File loading / saving system (prototype)
            if (ImGui::BeginMenu("File")) {
                ImGui::MenuItem("Load", "Ctrl+L");
                ImGui::MenuItem("Save", "Ctrl+S");
                ImGui::MenuItem("Save & Exit");
                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }
        // Debug
        if (selected_obj) {
            ImGui::Text("Selected object: %s", selected_obj->name.c_str());
        }
        // Display selected game objects
        for (int i = 0; i < active_object_count; i++) {
            ImGui::Text("Active: %s", active_objects[i]->name.c_str());
        }
        ImGui::End();
    }

    /**
     * Add a game object to the scene.
     * @param obj Game object reference
     */
    void add_game_object(GameObject::game_object *obj) {
        game_objects[game_object_count] = *obj;
        game_object_count++;
        Render::add_game_object(obj);
    }
}