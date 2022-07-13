/**
 * @author Charlie Davidson 
 * Created on 7/13/22.
 */

#include "behavior_manager.h"

namespace BehaviorManager {

    void init(BehaviorManager::behavior_manager *bh_manager) {
        bh_manager->solid_block = false;
        bh_manager->breakable_block = false;
        bh_manager->item_box = false;
        bh_manager->player = false;
        bh_manager->goomba = false;
        bh_manager->turtle = false;
        bh_manager->mushroom = false;
        bh_manager->fire_flower = false;
        bh_manager->pipe = false;
    }

    void init_solid_block(BehaviorManager::behavior_manager *bh_manager) {
        init(bh_manager);
        bh_manager->solid_block = true;
    }

    void init_breakable_block(BehaviorManager::behavior_manager *bh_manager) {
        init(bh_manager);
        bh_manager->breakable_block = true;
    }

    void init_item_box(BehaviorManager::behavior_manager *bh_manager) {
        init(bh_manager);
        bh_manager->item_box = true;
    }

    void init_player(BehaviorManager::behavior_manager *bh_manager) {
        init(bh_manager);
        bh_manager->player = true;
    }

    void init_goomba(BehaviorManager::behavior_manager *bh_manager) {
        init(bh_manager);
        bh_manager->goomba = true;
    }

    void init_turtle(BehaviorManager::behavior_manager *bh_manager) {
        init(bh_manager);
        bh_manager->turtle = true;
    }

    void init_mushroom(BehaviorManager::behavior_manager *bh_manager) {
        init(bh_manager);
        bh_manager->mushroom = true;
    }

    void init_fire_flower(BehaviorManager::behavior_manager *bh_manager) {
        init(bh_manager);
        bh_manager->fire_flower = true;
    }

    void init_pipe(BehaviorManager::behavior_manager *bh_manager) {
        init(bh_manager);
        bh_manager->pipe = true;
    }
}
