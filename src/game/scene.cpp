/**
 * @author Charlie Davidson
 * Created on 7/5/22.
 */

#include "scene.h"

#include "game/render/sprite_renderer.h"
#include "sprite_manager.h"
#include "editor/serialization/serializer.h"
#include "game/interface/main_menu_bar.h"

namespace Game {
    namespace Scene {

        static Game::SpriteManager::sprite_manager *game_objects;
        static int game_object_count;

        /**
         * Initialize the scene.
         */
        void init() {
            // Initialize game object array
            game_objects = new Game::SpriteManager::sprite_manager[10000];
            game_object_count = 0;

        }

        /**
         * Update the scene each frame.
         * @param dt Delta time
         */
        void update(double dt) {
            // Camera controls
            //Camera::update(dt);

            // Update mouse position
            //Mouse::calculate_world_coords();
        }

        /**
         * Add a game object to the scene.
         * @param obj Game object reference
         */
        void add_game_object(Game::SpriteManager::sprite_manager *obj) {
            if (game_object_count < 10000) {
                game_objects[game_object_count] = *obj;
                Game::SpriteRenderer::add_sprite(&game_objects[game_object_count]);
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
         * Get the game objects list.
         * @param game_objs Game objects reference
         * @param game_obj_count Count of game objects
         */
        void get_game_objects_list(Game::SpriteManager::sprite_manager **objects, int *object_count) {
            *objects = game_objects;
            *object_count = game_object_count;
        }

        /**
         * Clear the list of game objects.
         */
        void clear_game_objects() {
            game_object_count = 0;
        }

        void imgui() {
            Game::MainMenuBar::imgui();
        }
    }
}