/**
 * @author Charlie Davidson 
 * Created on 7/15/22.
 */

#pragma once

#include <vector>

#include "editor/game_object.h"

namespace LevelState {

    struct level_state {
        std::vector<Editor::GameObject::game_object> serialized_game_objects;
        std::vector<Editor::SpriteManager::sprite_manager> serialized_sprite_managers;

        template<class Archive>
        void serialize(Archive &archive) {
            archive(serialized_game_objects, serialized_sprite_managers);
        }
    };

    void init(LevelState::level_state *level_state);

}
