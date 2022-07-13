/**
 * @author Charlie Davidson
 * Created on 6/27/22.
 */

#pragma once

#include "core/sprite_manager.h"

namespace SpriteRenderer {

    void render();

    void add_sprite(SpriteManager::sprite_manager *obj);

    void remove_sprite(SpriteManager::sprite_manager *obj);

    void re_add_sprite(SpriteManager::sprite_manager *obj);

    void highlight_sprite(SpriteManager::sprite_manager **highlighted_obj);

    void highlight_sprites(SpriteManager::sprite_manager **highlighted_objects, int *highlighted_count, SpriteManager::sprite_manager *selection_box);

    void clear_render_batches();

}
