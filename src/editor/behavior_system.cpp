/**
 * @author Charlie Davidson 
 * Created on 7/14/22.
 */

#include "behavior_system.h"

namespace Editor {
    namespace BehaviorSystem {

        static Editor::BehaviorManager::behavior_manager *behavior_manager_list;
        static int behavior_manager_count;
        static Editor::BehaviorManager::behavior_manager *transient_behavior_manager_list;
        static int transient_behavior_manager_count;

        void init() {
            behavior_manager_list = new Editor::BehaviorManager::behavior_manager[10000];
            behavior_manager_count = 0;
            transient_behavior_manager_list = new Editor::BehaviorManager::behavior_manager[100];
            transient_behavior_manager_count = 0;
        }

        void reload() {
            behavior_manager_count = 0;
        }

        void init_behavior_manager(Editor::BehaviorManager::behavior_manager **bh_manager) {
            if (behavior_manager_count < 10000) {
                *bh_manager = &behavior_manager_list[behavior_manager_count];
                behavior_manager_count++;
            }
        }

        void init_transient_behavior_manager(Editor::BehaviorManager::behavior_manager **bh_manager) {
            if (transient_behavior_manager_count < 100) {
                *bh_manager = &transient_behavior_manager_list[transient_behavior_manager_count];
                transient_behavior_manager_count++;
            }
        }

        void remove_behavior_manager(Editor::BehaviorManager::behavior_manager *bh_manager) {
            bool found = false;
            for (int i = 0; i < behavior_manager_count; i++) {
                if (&behavior_manager_list[i] == bh_manager) {
                    found = true;
                }
                if (found && i < behavior_manager_count - 1) {
                    behavior_manager_list[i] = behavior_manager_list[i + 1];
                    behavior_manager_list[i].game_object->bh_manager = &behavior_manager_list[i];
                }
            }
            if (found) {
                behavior_manager_count--;
            }
        }

        void get_behavior_manager_list(Editor::BehaviorManager::behavior_manager **objects, int *object_count) {
            *objects = behavior_manager_list;
            *object_count = behavior_manager_count;
        }

        void get_transient_behavior_manager_list(Editor::BehaviorManager::behavior_manager **objects, int *object_count) {
            *objects = transient_behavior_manager_list;
            *object_count = transient_behavior_manager_count;
        }
    }
}