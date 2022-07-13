/**
 * @author Charlie Davidson
 * Created on 7/5/22.
 */

#include <cstdio>

#include "scene.h"

#include "core/render/sprite_renderer.h"
#include "editor/collision/chunk_manager.h"
#include "game_object.h"
#include "camera.h"
#include "mouse_listener.h"
#include "serializer.h"
#include "editor/object_manager.h"
#include "editor/main_menu_bar.h"
#include "editor/info_window.h"
#include "editor/object_picker.h"

namespace Scene {

    static GameObject::game_object *game_objects;
    static int game_object_count;
    static std::string level_filepath;

    /**
     * Initialize the scene.
     */
    void init() {

        level_filepath = "level.txt";

        // Initialize game object array
        game_objects = new GameObject::game_object[10000];
        game_object_count = 0;

        // Initialize objects
        ObjectManager::init();

        // Initialize ImGui elements
        MainMenuBar::init();
        ObjectPicker::init();

        Scene::load_level();

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

        // Update objects in the scene
        ObjectManager::update();

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
        // Main menu bar
        MainMenuBar::imgui();

        // Info window
        if (MainMenuBar::show_info_window()) {
            InfoWindow::imgui();
        }

        // Object picker
        if (MainMenuBar::show_object_picker()) {
            ObjectPicker::imgui();
        }
    }

    /**
     * Add a game object to the scene.
     * @param obj Game object reference
     */
    void add_game_object(GameObject::game_object *obj) {
        if (game_object_count < 10000) {
            game_objects[game_object_count] = *obj;
            SpriteRenderer::add_game_object(&game_objects[game_object_count]);
            game_object_count++;
        }
    }

    /**
     * Removes dead game objects from the scene.
     */
    void remove_game_objects() {
        int dead_count = 0;
        bool dead = false;
        for (int i = 0; i < game_object_count; i++) {
            if (i == game_object_count - dead_count) {
                break;
            }
            if (dead_count > 0) {
                game_objects[i] = game_objects[i + dead_count];
            }
            if (game_objects[i].dead) {
                dead = true;
                dead_count++;
            }
            if (dead) {
                dead = false;
                i--;
            }
        }
        game_object_count -= dead_count;
    }

    /**
     * Save the state of the level to a file.
     */
    void save_level() {
        Serializer::serialize_game_objects(level_filepath);
    }

    /**
     * Load a level from an existing file.
     */
    void load_level() {
        Serializer::deserialize_game_objects(level_filepath);
    }

    /**
     * Create a new level.
     */
    void new_level() {
        SpriteRenderer::clear_render_batches();
        ObjectManager::reload();
        ChunkManager::reload();

        printf("Created new level\n");
    }

    /**
     * Get the game objects list.
     * @param game_objs Game objects reference
     * @param game_obj_count Count of game objects
     */
    void get_game_objects_list(GameObject::game_object **objects, int *object_count) {
        *objects = game_objects;
        *object_count = game_object_count;
    }

    /**
     * Clear the list of game objects.
     */
    void clear_game_objects() {
        game_object_count = 0;
    }
}