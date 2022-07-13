/**
 * @author Charlie Davidson 
 * Created on 7/13/22.
 */

#pragma once

#include "sprite_manager.h"
#include "game/behavior_manager.h"
#include "game/physics/physics_manager.h"

namespace GameObject {

    struct game_object {
        int id;
        SpriteManager::sprite_manager *spr_manager;
        PhysicsManager::physics_manager *py_manager;
        BehaviorManager::behavior_manager *bh_manager;

    };

}
