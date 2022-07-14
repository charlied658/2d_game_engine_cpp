/**
 * @author Charlie Davidson 
 * Created on 7/13/22.
 */

#pragma once

#include "editor/sprite_manager.h"
#include "editor/behavior_manager.h"
#include "editor/physics_manager.h"

namespace Editor {
    namespace GameObject {

        struct game_object {
            int id;
            Editor::SpriteManager::sprite_manager *spr_manager;
            Editor::PhysicsManager::physics_manager *py_manager;
            Editor::BehaviorManager::behavior_manager *bh_manager;

        };

    }
}
