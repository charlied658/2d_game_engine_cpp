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

        void add_transient_sprite(Editor::SpriteManager::sprite_manager *obj);

        void highlight_sprite(Editor::GameObject::game_object **highlighted_obj);

        void highlight_sprites(Editor::GameObject::game_object **highlighted_objects, int *highlighted_count, Editor::GameObject::game_object *selection_box);

        void clear_render_batches();

    }
}
