/**
 * @author Charlie Davidson 
 * Created on 7/13/22.
 */

#include "behavior_manager.h"

namespace Editor {
    namespace BehaviorManager {

        void init(Editor::BehaviorManager::behavior_manager *bh_manager) {
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

        void init_solid_block(Editor::BehaviorManager::behavior_manager *bh_manager) {
            init(bh_manager);
            bh_manager->solid_block = true;
        }

        void init_breakable_block(Editor::BehaviorManager::behavior_manager *bh_manager) {
            init(bh_manager);
            bh_manager->breakable_block = true;
        }

        void init_item_box(Editor::BehaviorManager::behavior_manager *bh_manager) {
            init(bh_manager);
            bh_manager->item_box = true;
        }

        void init_player(Editor::BehaviorManager::behavior_manager *bh_manager) {
            init(bh_manager);
            bh_manager->player = true;
        }

        void init_goomba(Editor::BehaviorManager::behavior_manager *bh_manager) {
            init(bh_manager);
            bh_manager->goomba = true;
        }

        void init_turtle(Editor::BehaviorManager::behavior_manager *bh_manager) {
            init(bh_manager);
            bh_manager->turtle = true;
        }

        void init_mushroom(Editor::BehaviorManager::behavior_manager *bh_manager) {
            init(bh_manager);
            bh_manager->mushroom = true;
        }

        void init_fire_flower(Editor::BehaviorManager::behavior_manager *bh_manager) {
            init(bh_manager);
            bh_manager->fire_flower = true;
        }

        void init_pipe(Editor::BehaviorManager::behavior_manager *bh_manager) {
            init(bh_manager);
            bh_manager->pipe = true;
        }
    }
}
