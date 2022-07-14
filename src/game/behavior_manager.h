/**
 * @author Charlie Davidson 
 * Created on 7/13/22.
 */

#pragma once

namespace Game {
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

        void init(Game::BehaviorManager::behavior_manager *bh_manager);

        void init_solid_block(Game::BehaviorManager::behavior_manager *bh_manager);

        void init_breakable_block(Game::BehaviorManager::behavior_manager *bh_manager);

        void init_item_box(Game::BehaviorManager::behavior_manager *bh_manager);

        void init_player(Game::BehaviorManager::behavior_manager *bh_manager);

        void init_goomba(Game::BehaviorManager::behavior_manager *bh_manager);

        void init_turtle(Game::BehaviorManager::behavior_manager *bh_manager);

        void init_mushroom(Game::BehaviorManager::behavior_manager *bh_manager);

        void init_fire_flower(Game::BehaviorManager::behavior_manager *bh_manager);

        void init_pipe(Game::BehaviorManager::behavior_manager *bh_manager);

    }
}