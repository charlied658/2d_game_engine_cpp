/**
 * @author Charlie Davidson 
 * Created on 7/14/22.
 */

#pragma once

#include "sprite_manager.h"

namespace Editor {
    namespace SpriteSystem {

        void init();

        void reload();

        void init_sprite_manager(Editor::SpriteManager::sprite_manager **spr_manager);

        void remove_sprite_manager(Editor::SpriteManager::sprite_manager *spr_manager);

        void get_sprite_manager_list(Editor::SpriteManager::sprite_manager **objects, int *object_count);

    }
}