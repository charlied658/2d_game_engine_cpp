/**
 * @author Charlie Davidson 
 * Created on 7/13/22.
 */

#include "behavior_manager.h"

namespace Editor {
    namespace BehaviorManager {

        void init(Editor::BehaviorManager::behavior_manager *bh_manager) {
            bh_manager->is_solid_block = false;
            bh_manager->is_breakable_block = false;
            bh_manager->is_item_box = false;
            bh_manager->is_player = false;
            bh_manager->is_goomba = false;
            bh_manager->is_turtle = false;
            bh_manager->is_mushroom = false;
            bh_manager->is_fire_flower = false;
            bh_manager->is_pipe = false;
        }

        void init_solid_block(Editor::BehaviorManager::behavior_manager *bh_manager) {
            init(bh_manager);
            bh_manager->is_solid_block = true;
        }

        void init_breakable_block(Editor::BehaviorManager::behavior_manager *bh_manager) {
            init(bh_manager);
            bh_manager->is_breakable_block = true;
        }

        void init_item_box(Editor::BehaviorManager::behavior_manager *bh_manager) {
            init(bh_manager);
            bh_manager->is_item_box = true;
        }

        void init_player(Editor::BehaviorManager::behavior_manager *bh_manager) {
            init(bh_manager);
            bh_manager->is_player = true;
        }

        void init_goomba(Editor::BehaviorManager::behavior_manager *bh_manager) {
            init(bh_manager);
            bh_manager->is_goomba = true;
        }

        void init_turtle(Editor::BehaviorManager::behavior_manager *bh_manager) {
            init(bh_manager);
            bh_manager->is_turtle = true;
        }

        void init_mushroom(Editor::BehaviorManager::behavior_manager *bh_manager) {
            init(bh_manager);
            bh_manager->is_mushroom = true;
        }

        void init_fire_flower(Editor::BehaviorManager::behavior_manager *bh_manager) {
            init(bh_manager);
            bh_manager->is_fire_flower = true;
        }

        void init_pipe(Editor::BehaviorManager::behavior_manager *bh_manager) {
            init(bh_manager);
            bh_manager->is_pipe = true;
        }
    }
}
