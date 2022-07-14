/**
 * @author Charlie Davidson
 * Created on 6/27/22.
 */

#pragma once

#include "editor/sprite_manager.h"

namespace Editor {
    namespace SpriteRenderer {

        void render();

        void add_sprite(Editor::SpriteManager::sprite_manager *obj);

        void remove_sprite(Editor::SpriteManager::sprite_manager *obj);

        void re_add_sprite(Editor::SpriteManager::sprite_manager *obj);

        void highlight_sprite(Editor::SpriteManager::sprite_manager **highlighted_obj);

        void highlight_sprites(Editor::SpriteManager::sprite_manager **highlighted_objects, int *highlighted_count,
                               Editor::SpriteManager::sprite_manager *selection_box);

        void clear_render_batches();

    }
}
