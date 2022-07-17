/**
 * @author Charlie Davidson 
 * Created on 7/5/22.
 */

#pragma once

#include "sprite_manager.h"

namespace Game {
    namespace Scene {

        void init();

        void update(double dt);

        void add_game_object(Game::SpriteManager::sprite_manager *obj);

        void remove_game_objects();

        void get_game_objects_list(Game::SpriteManager::sprite_manager **objects, int *object_count);

        void clear_game_objects();

        void imgui();

    }
}