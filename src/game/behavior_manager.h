/**
 * @author Charlie Davidson 
 * Created on 7/13/22.
 */

#pragma once

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

    void init(BehaviorManager::behavior_manager *bh_manager);

    void init_solid_block(BehaviorManager::behavior_manager *bh_manager);

    void init_breakable_block(BehaviorManager::behavior_manager *bh_manager);

    void init_item_box(BehaviorManager::behavior_manager *bh_manager);

    void init_player(BehaviorManager::behavior_manager *bh_manager);

    void init_goomba(BehaviorManager::behavior_manager *bh_manager);

    void init_turtle(BehaviorManager::behavior_manager *bh_manager);

    void init_mushroom(BehaviorManager::behavior_manager *bh_manager);

    void init_fire_flower(BehaviorManager::behavior_manager *bh_manager);

    void init_pipe(BehaviorManager::behavior_manager *bh_manager);

}
