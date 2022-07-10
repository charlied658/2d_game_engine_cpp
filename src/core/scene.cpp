/**
 * @author Charlie Davidson
 * Created on 7/5/22.
 */

#include <cstdio>

#include "core/scene.h"

#include "imgui/imgui.h"

#include "render/render.h"
#include "core/game_object.h"
#include "core/sprite.h"
#include "core/spritesheet.h"
#include "core/camera.h"
#include "core/mouse_listener.h"
#include "core/window.h"
#include "editor/serialize.h"
#include "editor/select_objects.h"

namespace Scene {

    static GameObject::game_object *game_objects;
    static int game_object_count = 0;

    static string level_filepath;

    // Temporary test sprites and objects
    static Spritesheet::spritesheet spritesheet1, spritesheet2;
    static Sprite::sprite sprite1, sprite2, sprite3;
    static GameObject::game_object obj1, obj2, obj3;

    /**
     * Initialize the scene.
     */
    void init() {

        level_filepath = "level.txt";

        // Initialize game object array
        game_objects = new GameObject::game_object[1000];

        // Initialize selection objects
        SelectObjects::init();

        // =================== Generate temporary test objects
        // Create spritesheet
        Spritesheet::init(&spritesheet1, 14,2,"assets/images/spritesheet.png");
        Spritesheet::init(&spritesheet2, 4,1,"assets/images/turtle.png");

        // Create sprites
        sprite1 = Spritesheet::get_sprite(&spritesheet1, 0);
        sprite2 = Spritesheet::get_sprite(&spritesheet1, 14);
        sprite3 = Spritesheet::get_sprite(&spritesheet2, 0);

        // Generate game objects
        GameObject::init(&obj1, "obj1", glm::vec2 {2.5f, 1.0f}, glm::vec2 {0.5f, 0.5f}, 1, &sprite1);
        GameObject::init (&obj2, "obj2", glm::vec2 {3.0f, 1.0f}, glm::vec2 {0.5f, 0.5f}, 0, &sprite2);
        GameObject::init (&obj3, "obj3", glm::vec2 {2.0f, 1.0f}, glm::vec2 {0.5f, 0.75f}, 0, &sprite3);

        // Add game objects to the scene
        Scene::add_game_object(&obj1);
        Scene::add_game_object(&obj2);
        Scene::add_game_object(&obj3);
        // ===============================================

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

        // Update selected objects
        SelectObjects::update_selected_objects();

        // Key bindings for saving / loading
        if (Key::get_key_pressed(GLFW_KEY_LEFT_CONTROL) || Key::get_key_pressed(GLFW_KEY_LEFT_SUPER)) {
            if (Key::get_key_begin_press(GLFW_KEY_S)) {
                Scene::save_level();
            } else if (Key::get_key_begin_press(GLFW_KEY_L)) {
                Scene::load_level();
            } else if (Key::get_key_begin_press(GLFW_KEY_N)) {
                Scene::new_level();
            }
        }

    }

    /**
     * Update scene ImGui.
     */
    void imgui() {
        if (ImGui::BeginMainMenuBar()) {
            // Level loading / saving system
            if (ImGui::BeginMenu("File")) {
                if (ImGui::MenuItem("New", "Ctrl+N")) {
                    Scene::new_level();
                }
                if (ImGui::MenuItem("Load", "Ctrl+L")) {
                    Scene::load_level();
                }
                if (ImGui::MenuItem("Save", "Ctrl+S")) {
                    Scene::save_level();
                }
                if (ImGui::MenuItem("Save & Exit")) {
                    Scene::save_level();
                    Window::close_window();
                }
                if (ImGui::MenuItem("Exit")) {
                    Window::close_window();
                }
                ImGui::EndMenu();
            }
            // Copy / paste / delete objects
            if (ImGui::BeginMenu("Edit")) {
                if (ImGui::MenuItem("Copy", "Ctrl+C")) {
                    SelectObjects::copy_objects();
                }
                if (ImGui::MenuItem("Paste", "Ctrl+V")) {
                    SelectObjects::paste_objects();
                }
                if (ImGui::MenuItem("Delete")) {
                    SelectObjects::delete_objects();
                }
                ImGui::EndMenu();
            }
            ImGui::EndMainMenuBar();
        }
        ImGui::Begin("Info");
        if (ImGui::BeginTabBar("TabBar"))
        {
            if (ImGui::BeginTabItem("Objects"))
            {
                SelectObjects::imgui();
                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem("Debug"))
            {
                ImGui::Text("FPS: %f", Window::get_fps());
                ImGui::EndTabItem();
            }
            ImGui::EndTabBar();
        }
        ImGui::End();
    }

    /**
     * Add a game object to the scene.
     * @param obj Game object reference
     */
    void add_game_object(GameObject::game_object *obj) {
        if (game_object_count < 1000) {
            game_objects[game_object_count] = *obj;
            Render::add_game_object(&game_objects[game_object_count]);
            game_object_count++;
        }
    }

    /**
     * Removes dead game objects from the scene.
     * TODO: Make this function more efficient by making it perform only one pass - should be possible
     */
    void remove_game_objects() {
        bool keep_iterating = true;
        while (keep_iterating) {
            // Remove objects one at a time by shifting every other object over by 1
            bool found_obj = false;
            for (int i = 0; i < game_object_count; i++) {
                if (game_objects[i].dead) {
                    found_obj = true;
                }
                if (found_obj && i < game_object_count - 1) {
                    game_objects[i] = game_objects[i + 1];
                }
            }
            if (found_obj) {
                keep_iterating = true;
                game_object_count--;
            } else {
                keep_iterating = false;
            }
        }
    }

    /**
     * Save the state of the level to a file.
     */
    void save_level() {
        Serialize::serialize_game_objects(level_filepath);
    }

    /**
     * Load a level from an existing file.
     */
    void load_level() {
        Serialize::deserialize_game_objects(level_filepath);
    }

    /**
     * Create a new level.
     */
    void new_level() {
        Render::clear_render_batches();
        SelectObjects::reload();
        Scene::add_game_object(&obj1);
        Scene::add_game_object(&obj2);
        Scene::add_game_object(&obj3);
        printf("Created new level\n");
    }

    /**
     * Get the game objects list
     * @param game_objs Game objects reference
     * @param game_obj_count Count of game objects
     */
    void get_game_objects_list(GameObject::game_object **objects, int *object_count) {
        *objects = game_objects;
        *object_count = game_object_count;
    }

    /**
     * Clear the list of game objects
     */
    void clear_game_objects() {
        game_object_count = 0;
    }
}