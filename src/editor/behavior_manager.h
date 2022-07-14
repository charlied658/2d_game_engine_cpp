/**
 * @author Charlie Davidson 
 * Created on 7/13/22.
 */

#pragma once

namespace Editor {
    namespace BehaviorManager {

        struct behavior_manager {
            bool solid_block;
            bool breakable_block;
            bool item_box;
            bool player;
            bool goomba;
            bool turtle;
            bool mushroom;
            bool fire_flower;
            bool pipe;
        };

        void init(Editor::BehaviorManager::behavior_manager *bh_manager);

        void init_solid_block(Editor::BehaviorManager::behavior_manager *bh_manager);

        void init_breakable_block(Editor::BehaviorManager::behavior_manager *bh_manager);

        void init_item_box(Editor::BehaviorManager::behavior_manager *bh_manager);

        void init_player(Editor::BehaviorManager::behavior_manager *bh_manager);

        void init_goomba(Editor::BehaviorManager::behavior_manager *bh_manager);

        void init_turtle(Editor::BehaviorManager::behavior_manager *bh_manager);

        void init_mushroom(Editor::BehaviorManager::behavior_manager *bh_manager);

        void init_fire_flower(Editor::BehaviorManager::behavior_manager *bh_manager);

        void init_pipe(Editor::BehaviorManager::behavior_manager *bh_manager);

    }
}
