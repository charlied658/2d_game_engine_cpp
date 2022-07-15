/**
 * @author Charlie Davidson 
 * Created on 7/14/22.
 */

#pragma once

#include "behavior_manager.h"

namespace Editor {
    namespace BehaviorSystem {

        void init();

        void reload();

        void init_behavior_manager(Editor::BehaviorManager::behavior_manager **bh_manager);

        void remove_behavior_manager(Editor::BehaviorManager::behavior_manager *bh_manager);

    }
}
