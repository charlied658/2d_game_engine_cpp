/**
 * @author Charlie Davidson
 * Created on 7/5/22.
 */

#include <cstdio>

#include "scene.h"

#include "editor/game_object.h"
#include "editor/render/sprite_renderer.h"
#include "editor/collision/chunk_manager.h"
#include "sprite_manager.h"
#include "core/camera.h"
#include "core/mouse_listener.h"
#include "editor/serialization/serializer.h"
#include "editor/interface/object_manager.h"
#include "editor/interface/main_menu_bar.h"
#include "editor/interface/info_window.h"
#include "editor/interface/object_picker.h"
#include "editor/interface/properties_window.h"

#include "editor/sprite_system.h"
#include "editor/physics_system.h"
#include "editor/behavior_system.h"

namespace Editor {
    namespace Scene {

        static Editor::GameObject::game_object *game_objects;
        static int game_object_count;
        static std::string level_filepath;

        static Editor::GameObject::game_object *transient_game_objects;
        static int transient_game_object_count;

        static Editor::GameObject::game_object *active_game_object;

        /**
         * Initialize the scene.
         */
        void init() {

            level_filepath = "level.txt";

            // Initialize game object array
            game_objects = new Editor::GameObject::game_object[10000];
            game_object_count = 0;
            transient_game_objects = new Editor::GameObject::game_object[10000];
            transient_game_object_count = 0;

            // Initialize systems
            SpriteSystem::init();
            PhysicsSystem::init();
            BehaviorSystem::init();

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

            // Properties Window
            if (MainMenuBar::show_properties_window()) {
                PropertiesWindow::imgui();
            }
        }

        /**
         * Return memory location of next available game object.
         * @param obj Pointer to available memory
         */
        void init_game_object(Editor::GameObject::game_object **obj) {
            if (game_object_count < 10000) {
                *obj = &game_objects[game_object_count];
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
                    game_objects[i].spr_manager->game_object = &game_objects[i];
                    game_objects[i].py_manager->game_object = &game_objects[i];
                    game_objects[i].bh_manager->game_object = &game_objects[i];
                }
                if (game_objects[i].dead) {
                    SpriteSystem::remove_sprite_manager(game_objects[i].spr_manager);
                    PhysicsSystem::remove_physics_manager(game_objects[i].py_manager);
                    BehaviorSystem::remove_behavior_manager(game_objects[i].bh_manager);
                    dead = true;
                    dead_count++;
                }
                if (dead) {
                    dead = false;
                    i--;
                }
            }
            game_object_count -= dead_count;

            // Reset render batches since pointers are now invalid
            SpriteRenderer::clear_render_batches();
            for (int i = 0; i < game_object_count; i++) {
                SpriteRenderer::add_sprite(game_objects[i].spr_manager);
            }
        }

        void init_transient_game_object(Editor::GameObject::game_object **obj) {
            if (transient_game_object_count < 10000) {
                *obj = &transient_game_objects[transient_game_object_count];
                transient_game_object_count++;
            }
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
            Scene::clear_game_objects();
            SpriteRenderer::clear_render_batches();
            ObjectManager::reload();
            ChunkManager::reload();
            SpriteSystem::reload();
            PhysicsSystem::reload();
            BehaviorSystem::reload();

            GameObject::set_id(0);

            printf("Created new level\n");
        }

        void get_game_objects_list(Editor::GameObject::game_object **objects, int *object_count) {
            *objects = game_objects;
            *object_count = game_object_count;
        }

        void get_active_game_object(Editor::GameObject::game_object ***object) {
            *object = &active_game_object;
        }

        void get_transient_game_objects_list(Editor::GameObject::game_object **objects, int *object_count) {
            *objects = transient_game_objects;
            *object_count = transient_game_object_count;
        }

        void clear_game_objects() {
            game_object_count = 0;
        }
    }
}