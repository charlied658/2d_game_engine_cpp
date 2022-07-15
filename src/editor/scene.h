/**
 * @author Charlie Davidson 
 * Created on 7/5/22.
 */

#pragma once

#include "sprite_manager.h"
#include "game_object.h"

namespace Editor {
    namespace Scene {

        void init();

        void update(double dt);

        void imgui();

        void init_game_object(Editor::GameObject::game_object **obj);

        void remove_game_objects();

        void get_game_objects_list(Editor::GameObject::game_object **objects, int *object_count);

        void get_active_game_object(Editor::GameObject::game_object ***object);

        void save_level();

        void load_level();

        void new_level();

        void clear_game_objects();

    }
}