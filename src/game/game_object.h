/**
 * @author Charlie Davidson 
 * Created on 7/13/22.
 */

#pragma once

#include "game/sprite_manager.h"
#include "game/behavior_manager.h"
#include "game/physics_manager.h"

namespace Game {
    namespace GameObject {

        struct game_object {
            int id;
            Game::SpriteManager::sprite_manager *spr_manager;
            Game::PhysicsManager::physics_manager *py_manager;
            Game::BehaviorManager::behavior_manager *bh_manager;

        };

    }
}
