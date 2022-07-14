/**
 * @author Charlie Davidson
 * Created on 6/27/22.
 */

#pragma once

#include "game/sprite_manager.h"

namespace Game {
    namespace SpriteRenderer {

        void render();

        void add_sprite(Game::SpriteManager::sprite_manager *obj);

        void remove_sprite(Game::SpriteManager::sprite_manager *obj);

        void re_add_sprite(Game::SpriteManager::sprite_manager *obj);

        void clear_render_batches();

    }
}
