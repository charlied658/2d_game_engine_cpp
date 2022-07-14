/**
 * @author Charlie Davidson 
 * Created on 7/5/22.
 */

#pragma once

#include "sprite_manager.h"

namespace Editor {
    namespace Scene {

        void init();

        void update(double dt);

        void imgui();

        void add_game_object(Editor::SpriteManager::sprite_manager *obj);

        void remove_game_objects();

        void get_game_objects_list(Editor::SpriteManager::sprite_manager **objects, int *object_count);

        void save_level();

        void load_level();

        void new_level();

        void clear_game_objects();

    }
}