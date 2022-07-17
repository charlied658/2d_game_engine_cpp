/**
 * @author Charlie Davidson 
 * Created on 7/14/22.
 */

#include "physics_system.h"

namespace Editor {
    namespace PhysicsSystem {

        static Editor::PhysicsManager::physics_manager *physics_manager_list;
        static int physics_manager_count;
        static Editor::PhysicsManager::physics_manager *transient_physics_manager_list;
        static int transient_physics_manager_count;

        void init() {
            physics_manager_list = new Editor::PhysicsManager::physics_manager[10000];
            physics_manager_count = 0;
            transient_physics_manager_list = new Editor::PhysicsManager::physics_manager[100];
            transient_physics_manager_count = 0;
        }

        void reload() {
            physics_manager_count = 0;
        }

        void init_physics_manager(Editor::PhysicsManager::physics_manager **py_manager) {
            if (physics_manager_count < 10000) {
                *py_manager = &physics_manager_list[physics_manager_count];
                physics_manager_count++;
            }
        }

        void init_transient_physics_manager(Editor::PhysicsManager::physics_manager **py_manager) {
            if (transient_physics_manager_count < 100) {
                *py_manager = &transient_physics_manager_list[transient_physics_manager_count];
                transient_physics_manager_count++;
            }
        }

        void remove_physics_manager(Editor::PhysicsManager::physics_manager *py_manager) {
            bool found = false;
            for (int i = 0; i < physics_manager_count; i++) {
                if (&physics_manager_list[i] == py_manager) {
                    found = true;
                }
                if (found && i < physics_manager_count - 1) {
                    physics_manager_list[i] = physics_manager_list[i + 1];
                    physics_manager_list[i].game_object->py_manager = &physics_manager_list[i];
                }
            }
            if (found) {
                physics_manager_count--;
            }
        }

        void get_physics_manager_list(Editor::PhysicsManager::physics_manager **objects, int *object_count) {
            *objects = physics_manager_list;
            *object_count = physics_manager_count;
        }

        void get_transient_physics_manager_list(Editor::PhysicsManager::physics_manager **objects, int *object_count) {
            *objects = transient_physics_manager_list;
            *object_count = transient_physics_manager_count;
        }
    }
}
