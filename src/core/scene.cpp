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

    static GameObject::game_object game_objects[1000];
    static int game_object_count = 0;

    static unsigned int textureID1, textureID2;
    static Spritesheet::spritesheet spritesheet1, spritesheet2;
    static Sprite::sprite sprite1, sprite2, sprite3, sprite4;
    static GameObject::game_object obj1, obj2, obj3;
    static GameObject::game_object *selected_obj, *active_obj;

    bool begin_drag;
    static double start_x, start_y;
    static double obj_start_x, obj_start_y;

    static float color[4] = {1.0f, 1.0f, 1.0f, 1.0f};

    /**
     * Initialize the scene.
     */
    void init() {
        // Generate textures
        textureID1 = Texture::get_texture("assets/images/spritesheet.png")->textureID;

        // Create spritesheet
        spritesheet1 = Spritesheet::spritesheet {14,2,textureID1};

        // Create sprites
        sprite1 = Spritesheet::get_sprite(&spritesheet1, 0);
        sprite2 = Spritesheet::get_sprite(&spritesheet1, 14);

        // Generate game objects
        GameObject::init(&obj1, "obj1", glm::vec2 {2.0f, 1.0f}, glm::vec2 {1.0f, 1.0f}, 1, &sprite1);
        GameObject::init (&obj2, "obj2", glm::vec2 {3.0f, 1.0f}, glm::vec2 {1.0f, 1.0f}, 0, &sprite2);
        GameObject::init (&obj3, "obj3", glm::vec2 {3.0f, 1.0f}, glm::vec2 {1.0f, 1.0f}, -5, &sprite2);
        obj3.pickable = false;

        // Add game objects to the scene
        Scene::add_game_object(&obj1);
        Scene::add_game_object(&obj2);
        Scene::add_game_object(&obj3);

        selected_obj = nullptr;
        active_obj = nullptr;

        begin_drag = false;

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

        // Reset position of objects
        if (Key::get_key_pressed(GLFW_KEY_SPACE)) {
            GameObject::set_position(&obj1, glm::vec2 {2.0f, 1.0f});
            GameObject::set_position(&obj2, glm::vec2 {3.0f, 1.0f});
        }

        // Render shadow for active game object
        if (active_obj) {
            obj3.visible = true;
            GameObject::init(&obj3, "shadow", active_obj->position + glm::vec2 {0.02f,-0.02f}, active_obj->scale, -5, &active_obj->sprite);
            GameObject::set_color(&obj3, glm::vec4(0.0f,0.0f,0.0f,0.7f));
        } else {
            obj3.visible = false;
            GameObject::update_color(&obj3);
        }

        // Select a game object based on the mouse position
        if (!ImGuiLayer::want_mouse_capture()) {
            if (!Mouse::get_mouse_pressed(GLFW_MOUSE_BUTTON_LEFT)) {
                Render::select_game_object(&selected_obj);
            }
        } else {
            if (!Mouse::get_mouse_pressed(GLFW_MOUSE_BUTTON_LEFT)) {
                if (selected_obj) {
                    GameObject::set_selected(selected_obj, false);
                    selected_obj = nullptr;
                }
            }
        }

        // Set active game object
        if (Mouse::get_mouse_pressed(GLFW_MOUSE_BUTTON_LEFT)) {
            if (selected_obj) {
                if (active_obj != selected_obj) {
                    active_obj = selected_obj;
                    active_obj->active = true;
                    active_obj->is_dirty = true;
                } else {
                    if (!begin_drag) {
                        begin_drag = true;
                        active_obj->dragging = true;
                        start_x = Mouse::get_worldX();
                        start_y = Mouse::get_worldY();
                        obj_start_x = active_obj->position.x;
                        obj_start_y = active_obj->position.y;
                    }
                    GameObject::set_position(active_obj,
                                             glm::vec2 {Mouse::get_worldX() - start_x + obj_start_x, Mouse::get_worldY() - start_y + obj_start_y});
                }
            } else {
                if (active_obj) {
                    active_obj->active = false;
                    active_obj = nullptr;
                }
            }
        } else {
            begin_drag = false;
            if (selected_obj) {
                selected_obj->dragging = false;
                GameObject::update_color(selected_obj);
            }
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
        // Set color of active game object
        if (active_obj) {
            color[0] = active_obj->color.x;
            color[1] = active_obj->color.y;
            color[2] = active_obj->color.z;
            color[3] = active_obj->color.w;
            ImGui::ColorPicker4("Color Picker", color);
            GameObject::set_color(active_obj, glm::vec4{color[0], color[1], color[2], color[3]});
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